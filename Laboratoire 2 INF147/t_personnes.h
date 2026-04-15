#pragma once
/***********************************************************************************/
/*  T_PERSONNES.h - Gestion de la liste des personnes.                             */
/*                                                                                 */
/*  Ce module permet de cr�er la liste initiale de toutes les personnes et ensuite */
/*  de g�rer leur temps de sortie ainsi que leur temps perdu dans le syst�me.      */
/*  Conception : E.Th�, D.E.G.                                                     */
/***********************************************************************************/
#ifndef __T_PERS_H__
#define __T_PERS_H__

#include "t_liste_even.h"

/* donn�es pour 1 personne */
typedef struct {
	int tmp_arr, tmp_sortie; 	//le temps d�arriv�e et de sortie
	int tmp_perdu;  		    //le temps perdu (en secondes)
	int etage_arr, etage_dest;  //�tage d'arriv�e et de destination
} t_personne;


/***********************************************************************************/
//        D�clarations des fonctions publiques pour les personnes
/***********************************************************************************/

/* Fonction principale qui g�nere TOUS les evenements d'arriv�es des
   personnes � un �tage choisi al�atoirement et les ajoutes a la liste cha�n�e */
t_personne* init_personnes(int taille, t_liste liste_even, int tp_arr_max,
	const double prob_eta[], const double prob_dest[], int nb_etages);

/* Fonction-destructeur pour lib�rer le tableau des personnes et remettre le pointeur re�u � NULL */
void detruire_liste_pers(t_personne** liste_pers);

/* 2 fonctions informatrices pour les 2 champs-�tages d'une personne */
int  get_etage_arr(const t_personne*);
int  get_etage_dest(const t_personne*);

/* �tablir le temps de sortie de la personne et ensuite calculer son temps perdu */
void set_tmp_sortie(t_personne* personne, int temps);

/* Faire le calcul de la moyenne de temps perdu pour TOUTES les personnes */
double calculer_tmp_perdu(const t_personne liste_pers[], int taille);

#endif
