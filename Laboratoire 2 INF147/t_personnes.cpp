#include <stdlib.h>
#include "t_personnes.h"

/* Choisit un etage aleatoire selon le tableau de probabilites P[].
   Algorithme impose par le cahier des charges :
     alea  <- 0.99999 * rand() / RAND_MAX
     etage <- 0
     total <- P[etage]
     Tant que (alea > total) : etage++, total += P[etage]
     Retourner etage                                               */
static int choisir_etage(const double prob[], int nb_etages)
{
    double alea = 0.99999 * rand() / RAND_MAX;
    int    etage = 0;
    double total = prob[0];

    while (alea > total && etage < nb_etages - 1) {
        etage++;
        total += prob[etage];
    }
    return etage;
}

/* Genere TOUS les evenements BOUTON d'arrivee des personnes et les insere
   dans la liste chainee d'evenements. */
t_personne* init_personnes(int taille, t_liste liste_even, int tmp_arr_max,
    const double prob_eta[], const double prob_dest[], int nb_etages)
{
    t_personne* liste_pers = (t_personne*)malloc(taille * sizeof(t_personne));

    int temps_courant = 0; /* temps d'arrivee cumulatif */

    for (int i = 0; i < taille; i++) {

        /* Calculer le temps d'arrivee de cette personne */
        if (i == 0)
            temps_courant = 0;
        else
            temps_courant += (int)(0.99999 * rand() / RAND_MAX * tmp_arr_max);

        liste_pers[i].tmp_arr = temps_courant;
        liste_pers[i].tmp_sortie = 0;
        liste_pers[i].tmp_perdu = 0;

        /* Choisir l'etage d'arrivee selon PROB_ETAG[] */
        liste_pers[i].etage_arr = choisir_etage(prob_eta, nb_etages);

        /* Choisir l'etage-destination :
           - Si la personne est au RC (etage 0), utiliser PROB_DEST[]
           - Sinon, utiliser PROB_ETAG[]
           - Recommencer si l'etage-destination == etage d'arrivee */
        do {
            if (liste_pers[i].etage_arr == 0)
                liste_pers[i].etage_dest = choisir_etage(prob_dest, nb_etages);
            else
                liste_pers[i].etage_dest = choisir_etage(prob_eta, nb_etages);
        } while (liste_pers[i].etage_dest == liste_pers[i].etage_arr);

        /* Inserer l'evenement BOUTON dans la liste chainee */
        inserer_element(liste_even, BOUTON,
            liste_pers[i].etage_arr,
            i,
            temps_courant);
    }

    return liste_pers;
}

/* Libere le tableau des personnes et remet le pointeur a NULL */
void detruire_liste_pers(t_personne** liste_pers)
{
    free(*liste_pers);
    *liste_pers = NULL;
}

/* Retourne l'etage d'arrivee de la personne */
int get_etage_arr(const t_personne* per)
{
    return per->etage_arr;
}

/* Retourne l'etage-destination de la personne */
int get_etage_dest(const t_personne* per)
{
    return per->etage_dest;
}

/* Etablit le temps de sortie et calcule le temps perdu */
void set_tmp_sortie(t_personne* personne, int temps)
{
    personne->tmp_sortie = temps;
    personne->tmp_perdu = personne->tmp_sortie - personne->tmp_arr;
}

/* Calcule et retourne la moyenne de temps perdu pour toutes les personnes */
double calculer_tmp_perdu(const t_personne liste_pers[], int taille)
{
    double total = 0.0;
    for (int i = 0; i < taille; i++)
        total += liste_pers[i].tmp_perdu;
    return total / taille;
}