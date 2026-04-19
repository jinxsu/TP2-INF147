/*****************************************************************************/
/*	T_ASCENCEUR.cpp									   						 */
/*																			 */
/*	Description: Ce module gère la logique métier et l'état des ascenseurs.  */
/*               Il inclut la gestion des boutons d'appel, l'algorithme de   */
/*               déplacement (choix de direction/arrêt), et la gestion des   */
/*               embarquements/débarquements des passagers via des files.    */
/*  Conception : Zhang Rui Chen et Samuel Dero								 */
/*****************************************************************************/
#include "t_ascenseur.h"

void init_boutons(t_boutons boutons)
{
	for (int i = 0; i < NB_ETAGES; i++) {
		boutons[i].up = 0;
		boutons[i].down = 0;
	}
}

int bouton_en_attente(t_boutons boutons)
{
	for (int i = 0; i < NB_ETAGES; i++) {
		if (boutons[i].up == 1 || boutons[i].down == 1)
			return 1;
	}
	return 0;
}

int get_bouton(t_boutons boutons, int etage, t_mode mode)
{
	if (mode == UP)
		return boutons[etage].up;
	else
		return boutons[etage].down;
}

void set_bouton_up(t_bouton* bt) { bt->up = 1; }
void set_bouton_dn(t_bouton* bt) { bt->down = 1; }
void clr_bouton_up(t_bouton* bt) { bt->up = 0; }
void clr_bouton_dn(t_bouton* bt) { bt->down = 0; }

void init_asc(t_asc liste_asc[], int nb_asc)
{
	for (int i = 0; i < nb_asc; i++) {
		liste_asc[i].dispo = 1;
		liste_asc[i].etage = RC;
		liste_asc[i].mode = STOP;
		liste_asc[i].modesuiv = STOP;
		liste_asc[i].nbper = 0;
		for (int j = 0; j < NB_ETAGES; j++) {
			liste_asc[i].boutons[j] = 0;
			liste_asc[i].files_personnes[j] = init_file(CAPACITE);
		}
	}
}

void detruire_files_asc(t_asc liste_asc[], int nb_asc)
{
	for (int i = 0; i < nb_asc; i++)
		for (int j = 0; j < NB_ETAGES; j++)
			detruire_file(&liste_asc[i].files_personnes[j]);
}

t_mode get_mode(const t_asc* asc) { return asc->mode; }
t_mode get_modesuiv(const t_asc* asc) { return asc->modesuiv; }
int    get_eta_asc(const t_asc* asc) { return asc->etage; }
int    get_nb_pers(const t_asc* asc) { return asc->nbper; }
int    est_dispo(const t_asc* asc) { return asc->dispo; }
int    get_bt_asc(const t_asc* asc, int eta) { return asc->boutons[eta]; }

void set_dispo(t_asc* asc, int etat) { asc->dispo = etat; }
void set_eta_asc(t_asc* asc, int etage) { asc->etage = etage; }
void transfert_mode(t_asc* asc) { asc->modesuiv = asc->mode; }
void transfert_modesuiv(t_asc* asc) { asc->mode = asc->modesuiv; }

int file_vide(const t_asc* asc, int etage)
{
	return est_vide(asc->files_personnes[etage]);
}

int appelle_asc(t_asc liste_asc[], int nb_asc, int etage, t_boutons boutons)
{
	int asc_trouve = AUCUN;
	for (int i = 0; i < nb_asc; i++) {
		if (liste_asc[i].dispo == 1 && liste_asc[i].mode == STOP) {
			asc_trouve = i;
			break;
		}
	}
	if (asc_trouve == AUCUN)
		return AUCUN;

	t_asc* asc = &liste_asc[asc_trouve];

	if (asc->etage == etage) {
		if (boutons[etage].up == 1)
			asc->modesuiv = UP;
		else
			asc->modesuiv = DOWN;
	}
	else if (asc->etage < etage)
		asc->modesuiv = UP;
	else
		asc->modesuiv = DOWN;

	return asc_trouve;
}

/* CHOIX_MODE - Intelligence des ascenseurs.
   Regle d'arret : si l'ascenseur monte (modesuiv==UP) et que l'etage actuel a
   un bouton UP allume OU des gens dedans qui veulent monter -> STOP (ouvrir portes).
   Meme logique pour DOWN. Ainsi l'ascenseur s'arrete a chaque etage ou il y a
   une demande dans sa direction. */
t_mode choix_mode(t_asc* asc, t_boutons boutons, int etage)
{
	// 1. Priorité : arrêt à l’étage courant
	if (!est_vide(asc->files_personnes[etage])) {
		asc->mode = asc->modesuiv = STOP;
		return STOP;
	}

	if (asc->modesuiv == UP && get_bouton(boutons, etage, UP)) {
		asc->mode = asc->modesuiv = STOP;
		return STOP;
	}

	if (asc->modesuiv == DOWN && get_bouton(boutons, etage, DOWN)) {
		asc->mode = asc->modesuiv = STOP;
		return STOP;
	}

	// 2. Chercher demandes dans chaque direction
	int demande_haut = 0;
	int demande_bas = 0;

	// --- au-dessus ---
	for (int i = etage + 1; i < NB_ETAGES; i++) {
		if (!est_vide(asc->files_personnes[i]) ||
			get_bouton(boutons, i, UP)) {
			demande_haut = 1;
			break;
		}
	}

	// --- en dessous ---
	for (int i = etage - 1; i >= RC; i--) {
		if (!est_vide(asc->files_personnes[i]) ||
			get_bouton(boutons, i, DOWN)) {
			demande_bas = 1;
			break;
		}
	}

	// 3. Logique de direction (SCAN amélioré)

	if (asc->modesuiv == UP) {
		if (demande_haut) {
			asc->mode = asc->modesuiv = UP;
			return UP;
		}
		else if (demande_bas) {
			asc->mode = asc->modesuiv = DOWN;
			return DOWN;
		}
	}

	else if (asc->modesuiv == DOWN) {
		if (demande_bas) {
			asc->mode = asc->modesuiv = DOWN;
			return DOWN;
		}
		else if (demande_haut) {
			asc->mode = asc->modesuiv = UP;
			return UP;
		}
	}

	// 4. Cas STOP (ascenseur idle)
	if (demande_haut) {
		asc->mode = asc->modesuiv = UP;
		return UP;
	}
	if (demande_bas) {
		asc->mode = asc->modesuiv = DOWN;
		return DOWN;
	}

	// 5. Rien à faire
	asc->mode = asc->modesuiv = STOP;
	return STOP;
}


int vider_file_asc(t_asc* asc, int etage, t_personne liste_pers[], int temps)
{
	int count = 0;
	int no_pers;

	while (!est_vide(asc->files_personnes[etage])) {
		enlever_debut(asc->files_personnes[etage], &no_pers);
		set_tmp_sortie(&liste_pers[no_pers], temps);
		asc->nbper--;
		count++;
	}
	asc->boutons[etage] = 0;
	return count;
}

int transfert_file(t_asc* asc, t_file file, t_personne liste_pers[])
{
	int count = 0;
	int no_pers;
	int dest_etage;

	while (!est_vide(file) && asc->nbper < CAPACITE) {
		enlever_debut(file, &no_pers);
		dest_etage = get_etage_dest(&liste_pers[no_pers]);
		ajouter_fin(asc->files_personnes[dest_etage], no_pers);
		asc->boutons[dest_etage] = 1;
		asc->nbper++;
		count++;
	}
	return count;
}