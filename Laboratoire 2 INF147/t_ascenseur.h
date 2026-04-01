#pragma once
/***********************************************************************************/
/*  T_ASCENSEUR.h - Gestion du syst�me des ascenseurs + Boutons-�tages             */
/*                                                                                 */
/*  Ce module va d�finir toutes les fonctionnalit�s du syst�me d'ascenseurs d'un   */
/*  b�timent incluant la gestion des boutons d'appel � chaque �tage.               */
/*  Conception : E.Th�, D.E.G.                                                     */
/***********************************************************************************/
#if !defined (NB_ETAGES)

#include "FILE_LIB.h"
#include "t_personne.h"

#define NB_ETAGES  6		//6 �tages (le b�timent n'a pas d'�tages au sous-sol)
#define CAPACITE  12		//12 personnes MAX dans un ascenseur
#define RC         0		//rez-de-chauss�e
#define AUCUN     -1		//constante � retourner si nous avons aucun ascenseur de disponible

/***********************************************************************************/
//d�clarations pour les boutons-�tages
/***********************************************************************************/
typedef struct {
	int up, down;     //2 boutons par �tage (1 = pes�, 0 - non-pes�)
} t_bouton;

typedef t_bouton t_boutons[NB_ETAGES];  //type-tableau des 6 paires de boutons

/***********************************************************************************/
//d�clarations pour les ascenseurs
/***********************************************************************************/
typedef enum { STOP, UP, DOWN } t_mode;   //3 modes d'op�rations

//type-struct d'un ascenseur
typedef struct {
	int dispo;							//1 = disponible, 0 = non
	int etage;							//�tage actuel
	t_mode  mode, modesuiv;				//le mode actuel et le prochain mode
	int boutons[NB_ETAGES];				//les 6 boutons-�tage situ�s DANS l'ascenseur
	t_file  files_personnes[NB_ETAGES];	//6 files de personnes (1 file pour chaque �tage)
	int nbper;							//nb. total de personnes dans l'ascenceur 
} t_asc;

/***********************************************************************************/
//        D�clarations des fonctions publiques pour les boutons-�tages
/***********************************************************************************/
/* va �teindre tous les boutons */
void init_boutons(t_boutons boutons);

/* v�rifie si un bouton est encore pes� sur un �tage du batiment (1= oui, 0= aucun) */
int  bouton_en_attente(t_boutons boutons);

/* fonction informatrice qui renvoie l'�tat du bouton UP/DOWN (selon le mode) � l'�tage re�u.
   param�tres :
	 boutons - la liste des boutons-�tage
	 etage - l'�tage en question
	 mode - soit UP ou DOWN pour inquer quel bouton on int�rroge

   retour : 1 si le bouton en question est pes�, 0 sinon   */
int  get_bouton(t_boutons boutons, int etage, t_mode mode);

void set_bouton_up(t_bouton* bt);    //allumer un bouton-UP
void set_bouton_dn(t_bouton* bt);    //allumer un bouton-DN
void clr_bouton_up(t_bouton* bt);    //�teindre un bouton-UP
void clr_bouton_dn(t_bouton* bt);    //�teindre un bouton-DN


/***********************************************************************************/
//          D�clarations des fonctions publiques pour les ascenseurs
/***********************************************************************************/
/* initialise tous les ascenseurs au RC, va aussi cr�er leurs 6 files internes
   avec une taille = CAPACITE pour chacune d'elles.  */
void init_asc(t_asc liste_asc[], int nb_asc);

/* va d�truire toutes les files d'attente internes de tous les ascenseurs */
void detruire_files_asc(t_asc liste_asc[], int nb_asc);


/* Les fonctions informatrices */

t_mode get_mode(const t_asc* asc);			  //retourne le mode actuel de l'ascenseur

t_mode get_modesuiv(const t_asc* asc);		  //retourne le mode suivant de l'ascenseur

int get_eta_asc(const t_asc* asc);			  //retourne l'�tage actuel de l'ascenseur

int get_nb_pers(const t_asc* asc);			  //retourne le nb. de personnes dans l'ascenseur

int est_dispo(const t_asc* asc);              //retourne 1 si cet ascenseur est disponible, 0 sinon

int get_bt_asc(const t_asc* asc, int etage);  //retourne l'�tat du bouton #etage de l'ascenseur

/* Les fonctions mutatrices */

void set_dispo(t_asc* asc, int etat);		  //modifier sa disponibilit� � "etat" (= 0 ou 1)

void set_eta_asc(t_asc* asc, int etage);	  //modifier son etage actuel � "etage"

void transfert_mode(t_asc* asc);			  //pour copier .mode --> .modesuiv

void transfert_modesuiv(t_asc* asc);		  //pour copier .modesuiv --> .mode


/* v�rifie si la file interne #�tage de l'ascenseur re�u est vide (1= vide, 0= non) */
int file_vide(const t_asc* asc, int etage);

/* Fonction qui fait appel � un ascenseur disponible en mode arr�t pour l'"�tage" re�u.
   Si on trouve un ascenseur disponible, la fonction va �tablir son mode suivant selon
   l'�tage re�u.  Si l'ascenseur est situ� au M�ME �tage, le mode suivant sera d�termin�
   par l'�tat des 2 boutons (UP/DOWN) pes�s � ce m�me �tage (on priorisera la direction UP),
   sinon on va l'envoyer dans la bonne direction pour atteindre l"�tage" d�sir�.

   Param�tres :
	 liste_asc - la liste des ascenseurs
	 nb_asc - nombre d'ascenseurs dans la liste
	 etage - l'�tage d'o� provient l'appel d'un ascenseur (�tage du bouton)
	 boutons - liste des boutons aux �tages

   Retour : La fonction retourne le num�ro de l'ascenseur trouv�,
			si aucun ascenseur est disponible, la fonction va renvoyer la valeur AUCUN. */
int appelle_asc(t_asc liste_asc[], int nb_asc, int etage, t_boutons boutons);

/* Fonction qui choisira le mode actuel ET le mode suivant d'un ascenseur en fonction de l'�tat
   des "boutons" et de l'"�tage" actuel. Cette fonction implante l'INTELLIGENCE DES ASCENSEURS.

   Param�tres :
	 asc - pointeur vers l'ascenseur � g�rer
	 boutons - liste des boutons aux �tages
	 etage - l'�tage actuel de l'�v�nement

   Retour : La fonction retourne le mode actuel (soit STOP, UP ou DOWN).
			Dans le cas du mode STOP elle va aussi �tablir son mode suivant.  */
t_mode choix_mode(t_asc* asc, t_boutons boutons, int etage);

/* Fonction qui fera le transfert des gens dans l'ascenseur re�u qui veulent SORTIR � l'�tage re�u.
   Va aussi �tablir le temps de sortie de chacun de ces gens et calculer leur temps perdu.
   Suite � ce transfert, le bouton de cet �tage dans l'ascenseur sera �teint.

	Param�tres :
	 asc - pointeur vers l'ascenseur � g�rer
	 etage - l'�tage actuel de l'�v�nement
	 liste_pers - tableau des personnes
	 temps - le temps actuel de l'�v�nement-PORTES (sera le temps de sortie des personnes)

   Retour : retourne le nombre de personnes qui sont sortis de l'ascenseur. */
int vider_file_asc(t_asc* asc, int etage, t_personne liste_pers[], int temps);

/* Fonction qui fera le transfert des gens qui attendent � un �tage DANS l'ascenseur re�u.
   Chaque personne sera ajout�e � une file interne de l'ascenseur selon son �tage-destination.
   Il va aussi appuyer sur le bouton de son �tage-destination une fois dans l'ascenseur.
   Si le nombre de personnes dans l'ascenseur atteint sa capacit�, on arr�te le transfert.

   Param�tres :
	 asc - pointeur vers l'ascenseur � remplir
	 file - la file d'attente des gens qui veulent prendre cet ascenseur
	 liste_pers - tableau des personnes (permettra de r�cup�rer l'�tage-dest. d'une personne)

   Retour : retourne le nombre de personnes qui sont rentr�s dans l'ascenseur   */
int transfert_file(t_asc* asc, t_file file, t_personne liste_pers[]);

#endif
