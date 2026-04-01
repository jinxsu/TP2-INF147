#pragma once
/*********************************************************************/
/*	T_EVEN.h   									   				     */
/*																	 */
/*	Description: Module qui d�fini un �v�nement	chronologique dans   */
/*               le temps. Il existe 4 types d'�v�nements possibles. */
/*  Conception : E.Th�, D.E.G.                                       */
/*********************************************************************/
#pragma once

/************* D�CLARATIONS DE TYPES *****************/
//les 4 type d'evenements
typedef enum { BOUTON, DEPART, PORTES, ARRIVEE } t_type_even;

//partie-donn�es d'un noeud de la liste cha�n�e
typedef struct {
	t_type_even evenement;  //l'�venement (enum)
	int etage;              //�tage actuel OU �tage-destination
	int no;                 //position-tableau d'un ascenseur OU d'une personne
	int temps;              //le temps pr�vu de l'�v�nement en secondes
} t_even;				//type structur� pour un �v�nement chronologique

/************** FONCTION CONSTRUCTEUR ****************/
//Fonction qui initialise et retourne un nouvel �v�nement avec les 4 param�tres re�us
t_even init_even(t_type_even type_e, int eta, int no, int temps);


/************* FONCTIONS INFORMATRICES ***************/
t_type_even get_type(const t_even* even);  //Obtenir le type de cet �v�nement

int get_etage(const t_even* even);		   //Obtenir l'�tage de cet �v�nement

int get_no(const t_even* even);			   //Obtenir le num�ro d'ascenseur/personne de l'�v�nement

int get_temps(const t_even* even);         //Obtenir le temps de cet �v�nement

