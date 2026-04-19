/*********************************************************************/
/*	T_EVEN.cpp									   				     */
/*																	 */
/*	Description: Module qui défini un évènement	chronologique dans   */
/*               le temps. Il existe 4 types d'évènements possibles. */
/*  Conception : E.Thé, D.E.G.                                       */
/*********************************************************************/
#include "t_even.h"

/********************* LA FONCTION CONSTRUCTEUR **********************/
//Fonction qui initialise un nouvel évènement avec les 4 paramètres reçus
t_even init_even(t_type_even type_e, int eta, int no, int temps) {
	t_even nouv_even;

	nouv_even.evenement = type_e;
	nouv_even.etage = eta;
	nouv_even.no = no;
	nouv_even.temps = temps;

	return nouv_even;
}

/******************* LES 4 FONCTIONS INFORMATRICES *******************/
t_type_even get_type(const t_even* even) {
	return even->evenement;
}

int get_etage(const t_even* even) {
	return even->etage;
}

int get_no(const t_even* even) {
	return even->no;
}

int get_temps(const t_even* even) {
	return even->temps;
}

/*********************************************************************/