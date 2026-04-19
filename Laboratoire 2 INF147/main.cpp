/*************************************************************************/
/*	MAIN.cpp									   				         */
/*																		 */
/*	Description: Main program for the elevator simulation.		         */
/*               Manages events, elevators, and passengers.	             */
/*	Conception : Zhang Rui Chen et Samuel Dero							 */
/*************************************************************************/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>

#include "File_Lib.h"
#include "t_even.h"
#include "t_liste_even.h"
#include "t_personnes.h"
#include "t_ascenseur.h"
#include "Affichage.h"

/***********************************************************************************/
/* Constantes globales de la simulation                                            */
/***********************************************************************************/
#define NB_ASC      3
#define TMP_DEPL    2
#define TMP_ARRET   5
#define NB_PERS  1000
#define MAX_FILE   40
#define TMP_ARRMAX 20
#define TMP_TOT 10800

const double PROB_ETAG[NB_ETAGES] = { 0.5, 0.1, 0.1, 0.1, 0.1, 0.1 };
const double PROB_DEST[NB_ETAGES] = { 0, 0.05, 0.2, 0.3, 0.3, 0.15 };

/***********************************************************************************/
/* Prototypes                                                                      */
/***********************************************************************************/
void traitement_even_bouton(t_even even, t_personne liste_pers[],
    t_files_att files_att[], t_boutons boutons);
void traitement_even_depart(t_even even, int temps,
    t_asc liste_asc[], t_liste liste_evenements);
void traitement_even_portes(t_even even, int temps,
    t_asc liste_asc[], t_boutons boutons,
    t_personne liste_pers[], t_files_att files_att[],
    t_liste liste_evenements, int nb_sortis[]);
void traitement_even_arrivee(t_even even, int temps,
    t_asc liste_asc[], t_boutons boutons,
    t_liste liste_evenements);
void verifier_boutons_etage(int temps, t_boutons boutons,
    t_asc liste_asc[], t_liste liste_evenements);


/***********************************************************************************/
/* main                                                                            */
/***********************************************************************************/
int main(void)
{
   

    t_liste      liste_evenements;
    t_personne* liste_pers;
    t_asc        liste_asc[NB_ASC];
    t_boutons    boutons;
    t_files_att  files_att[NB_ETAGES];
    int          nb_sortis[NB_ETAGES];
    int          tmp_delai = 50;
    FILE* logfile;

    logfile = fopen("simulation.log", "w");
    liste_evenements = creer_liste();

    for (int i = 0; i < NB_ETAGES; i++) {
        files_att[i].file_up = init_file(MAX_FILE);
        files_att[i].file_dn = init_file(MAX_FILE);
        nb_sortis[i] = 0;
    }

    init_boutons(boutons);
    init_asc(liste_asc, NB_ASC);

    liste_pers = init_personnes(NB_PERS, liste_evenements, TMP_ARRMAX,
        PROB_ETAG, PROB_DEST, NB_ETAGES);

    aff_batiment();
    for (int i = 0; i < NB_ASC; i++)
        dessiner_asc(&liste_asc[i], i, STOP);
    aff_boutons(boutons);

    while (lire_taille_liste(liste_evenements) > 0) {
        t_even even;
        lire_premier_element(liste_evenements, &even);
        retirer_premier_element(liste_evenements);

        int temps = get_temps(&even);
        if (temps > TMP_TOT)
            break;

        aff_temps(temps);

        if (logfile) {
            const char* nom_even[] = { "BOUTON ", "DEPART ", "PORTES ", "ARRIVEE" };
            fprintf(logfile, "%s: Etage %d, No. %d, Temps: %d\n",
                nom_even[get_type(&even)],
                get_etage(&even),
                get_no(&even),
                temps);
        }

        switch (get_type(&even)) {
        case BOUTON:
            traitement_even_bouton(even, liste_pers, files_att, boutons);
            verifier_boutons_etage(temps, boutons, liste_asc, liste_evenements);
            break;
        case DEPART:
            traitement_even_depart(even, temps, liste_asc, liste_evenements);
            break;
        case PORTES:
            traitement_even_portes(even, temps, liste_asc, boutons,
                liste_pers, files_att, liste_evenements, nb_sortis);
            verifier_boutons_etage(temps, boutons, liste_asc, liste_evenements);
            break;
        case ARRIVEE:
            traitement_even_arrivee(even, temps, liste_asc, boutons, liste_evenements);
            break;
        }

        if (_kbhit()) {
            int touche = _getch();
            if (touche == 27)
                break;
            else if (touche == 'L' || touche == 'l') tmp_delai = 500;
            else if (touche == 'M' || touche == 'm') tmp_delai = 50;
            else if (touche == 'R' || touche == 'r') tmp_delai = 5;
        }
        delai(tmp_delai);
    }

    double moy = calculer_tmp_perdu(liste_pers, NB_PERS);
    aff_moyenne(moy);

    if (logfile) fclose(logfile);
    for (int i = 0; i < NB_ETAGES; i++) {
        detruire_file(&files_att[i].file_up);
        detruire_file(&files_att[i].file_dn);
    }
    detruire_files_asc(liste_asc, NB_ASC);
    detruire_liste_pers(&liste_pers);
    detruire_liste(&liste_evenements);
    return 0;
}

/***********************************************************************************/
/* A. traitement_even_bouton                                                       */
/***********************************************************************************/
void traitement_even_bouton(t_even even, t_personne liste_pers[],
    t_files_att files_att[], t_boutons boutons)
{
    int no_pers = get_no(&even);
    int etage_arr = get_etage_arr(&liste_pers[no_pers]);
    int etage_dst = get_etage_dest(&liste_pers[no_pers]);

    if (etage_dst > etage_arr) {
        ajouter_fin(files_att[etage_arr].file_up, no_pers);
        aff_file(files_att[etage_arr].file_up, etage_arr, UP);
        set_bouton_up(&boutons[etage_arr]);
    }
    else {
        ajouter_fin(files_att[etage_arr].file_dn, no_pers);
        aff_file(files_att[etage_arr].file_dn, etage_arr, DOWN);
        set_bouton_dn(&boutons[etage_arr]);
    }
    aff_boutons(boutons);
}

/***********************************************************************************/
/* B. traitement_even_depart                                                       */
/***********************************************************************************/
void traitement_even_depart(t_even even, int temps,
    t_asc liste_asc[], t_liste liste_evenements)
{
    int    no_asc = get_no(&even);
    t_asc* asc = &liste_asc[no_asc];
    int    etage = get_eta_asc(asc);

    transfert_modesuiv(asc);   /* .modesuiv --> .mode */

    int prochain_etage;
    if (asc->mode == UP)
        prochain_etage = etage + 1;
    else
        prochain_etage = etage - 1;

    /* Verification des bornes : ne jamais sortir de [RC, NB_ETAGES-1] */
    if (prochain_etage < RC)           prochain_etage = RC;
    if (prochain_etage >= NB_ETAGES)   prochain_etage = NB_ETAGES - 1;

    inserer_element(liste_evenements, ARRIVEE, prochain_etage, no_asc,
        temps + TMP_DEPL);

    effacer_asc(etage, no_asc);
}

/***********************************************************************************/
/* C. traitement_even_portes                                                       */
/***********************************************************************************/
void traitement_even_portes(t_even even, int temps,
    t_asc liste_asc[], t_boutons boutons,
    t_personne liste_pers[], t_files_att files_att[],
    t_liste liste_evenements, int nb_sortis[])
{
    int    no_asc = get_no(&even);
    t_asc* asc = &liste_asc[no_asc];
    int    etage = get_eta_asc(asc);

    /* Verification de securite : etage doit etre dans les bornes */
    if (etage < RC || etage >= NB_ETAGES) return;

    dessiner_asc(asc, no_asc, STOP);

    /* 1. Faire sortir les gens qui arrivent a destination */
    int nb_sortis_ici = vider_file_asc(asc, etage, liste_pers, temps);
    nb_sortis[etage] += nb_sortis_ici;

    /* 2. Faire entrer les gens selon la direction de l'ascenseur */
    int nb_entres = 0;

    if (asc->modesuiv == UP) {
        nb_entres = transfert_file(asc, files_att[etage].file_up, liste_pers);
        aff_file(files_att[etage].file_up, etage, UP);
        if (est_vide(files_att[etage].file_up))
            clr_bouton_up(&boutons[etage]);
    }
    else if (asc->modesuiv == DOWN) {
        nb_entres = transfert_file(asc, files_att[etage].file_dn, liste_pers);
        aff_file(files_att[etage].file_dn, etage, DOWN);
        if (est_vide(files_att[etage].file_dn))
            clr_bouton_dn(&boutons[etage]);
    }
    else {
        /* modesuiv == STOP : prendre UP en priorite, sinon DOWN */
        if (!est_vide(files_att[etage].file_up)) {
            asc->modesuiv = UP;
            nb_entres = transfert_file(asc, files_att[etage].file_up, liste_pers);
            aff_file(files_att[etage].file_up, etage, UP);
            if (est_vide(files_att[etage].file_up))
                clr_bouton_up(&boutons[etage]);
        }
        else if (!est_vide(files_att[etage].file_dn)) {
            asc->modesuiv = DOWN;
            nb_entres = transfert_file(asc, files_att[etage].file_dn, liste_pers);
            aff_file(files_att[etage].file_dn, etage, DOWN);
            if (est_vide(files_att[etage].file_dn))
                clr_bouton_dn(&boutons[etage]);
        }
    }

    aff_boutons(boutons);
    dessiner_asc(asc, no_asc, asc->modesuiv);
    aff_sorties(nb_sortis[etage], etage);

    /* 3. Remettre disponible si vide */
    if (asc->nbper == 0 &&
        est_vide(files_att[etage].file_up) &&
        est_vide(files_att[etage].file_dn)) {
        set_dispo(asc, 1);
    }

    /* 4. DEPART normal ou garder les PORTES ouvertes */
    int total_transfert = nb_sortis_ici + nb_entres;
    if (total_transfert < CAPACITE / 2) {
        inserer_element(liste_evenements, DEPART, etage, no_asc,
            temps + TMP_ARRET);
    }
    else {
        inserer_element(liste_evenements, PORTES, etage, no_asc,
            temps + TMP_ARRET);
    }
}

/***********************************************************************************/
/* D. traitement_even_arrivee                                                      */
/***********************************************************************************/
void traitement_even_arrivee(t_even even, int temps,
    t_asc liste_asc[], t_boutons boutons,
    t_liste liste_evenements)
{
    int    no_asc = get_no(&even);
    int    etage_dest = get_etage(&even);
    t_asc* asc = &liste_asc[no_asc];

    /* Verification des bornes : forcer dans [RC, NB_ETAGES-1] */
    if (etage_dest < RC)          etage_dest = RC;
    if (etage_dest >= NB_ETAGES)  etage_dest = NB_ETAGES - 1;

    set_eta_asc(asc, etage_dest);
    transfert_mode(asc);   /* .mode --> .modesuiv */

    t_mode nouveau_mode = choix_mode(asc, boutons, etage_dest);

    dessiner_asc(asc, no_asc, nouveau_mode);

    if (nouveau_mode == STOP) {
        inserer_element(liste_evenements, PORTES, etage_dest, no_asc,
            temps + TMP_ARRET);
    }
    else {
        int prochain_etage = (nouveau_mode == UP) ? etage_dest + 1 : etage_dest - 1;

        /* Verification des bornes sur le prochain etage */
        if (prochain_etage < RC)          prochain_etage = RC;
        if (prochain_etage >= NB_ETAGES)  prochain_etage = NB_ETAGES - 1;

        inserer_element(liste_evenements, ARRIVEE, prochain_etage, no_asc,
            temps + TMP_DEPL);
        delai(30);
        effacer_asc(etage_dest, no_asc);
    }
}

/***********************************************************************************/
/* E. verifier_boutons_etage                                                       */
/***********************************************************************************/
void verifier_boutons_etage(int temps, t_boutons boutons,
    t_asc liste_asc[], t_liste liste_evenements)
{
    if (!bouton_en_attente(boutons))
        return;

    for (int i = NB_ETAGES - 1; i >= RC; i--) {
        if (boutons[i].up || boutons[i].down) {
            int no_asc = appelle_asc(liste_asc, NB_ASC, i, boutons);
            if (no_asc != AUCUN) {
                set_dispo(&liste_asc[no_asc], 0);
                int etage_asc = get_eta_asc(&liste_asc[no_asc]);
                if (etage_asc == i) {
                    inserer_element(liste_evenements, PORTES, i, no_asc,
                        temps + TMP_DEPL);
                }
                else {
                    inserer_element(liste_evenements, DEPART, etage_asc, no_asc,
                        temps + TMP_DEPL);
                }
            }
        }
    }
}

