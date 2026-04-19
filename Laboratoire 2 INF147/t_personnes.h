#pragma once
/***********************************************************************************/
/*  T_PERSONNES.h - Gestion de la liste des personnes.                             */
/*                                                                                 */
/*  Ce module permet de créer la liste initiale de toutes les personnes et ensuite */
/*  de gérer leur temps de sortie ainsi que leur temps perdu dans le système.      */
/*  Conception :  E.Thé, D.E.G.                                     */
/***********************************************************************************/
#ifndef __T_PERS_H__
#define __T_PERS_H__

#include "t_liste_even.h"

/* données pour 1 personne */
typedef struct {
	int tmp_arr, tmp_sortie; 	//le temps d'arrivée et de sortie
	int tmp_perdu;  		    //le temps perdu (en secondes)
	int etage_arr, etage_dest;  //étage d'arrivée et de destination
} t_personne;


/***********************************************************************************/
//        Déclarations des fonctions publiques pour les personnes
/***********************************************************************************/

/* Fonction principale qui génere TOUS les evenements d'arrivées des
   personnes à un étage choisi aléatoirement et les ajoutes a la liste chaînée */
t_personne* init_personnes(int taille, t_liste liste_even, int tp_arr_max,
	const double prob_eta[], const double prob_dest[], int nb_etages);

/* Fonction-destructeur pour libérer le tableau des personnes et remettre le pointeur reçu à NULL */
void detruire_liste_pers(t_personne** liste_pers);

/* 2 fonctions informatrices pour les 2 champs-étages d'une personne */
int  get_etage_arr(const t_personne*);
int  get_etage_dest(const t_personne*);

/* établir le temps de sortie de la personne et ensuite calculer son temps perdu */
void set_tmp_sortie(t_personne* personne, int temps);

/* Faire le calcul de la moyenne de temps perdu pour TOUTES les personnes */
double calculer_tmp_perdu(const t_personne liste_pers[], int taille);

#endif
