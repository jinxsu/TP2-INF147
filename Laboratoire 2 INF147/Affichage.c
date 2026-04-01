/***********************************************************************************/
/*  AFFICHAGE.CPP - Module d'affichage                                             */
/*                                                                                 */
/*  Ce module va définir toutes les fonctionnalités d'affichage du système         */
/*  d'ascenseurs incluant l'état des boutons, les files d'attentes et le nombre    */
/*  de personnes sortis à chaque étage.                                            */
/*  Conception : Nov. 2019, E.Thé, S.E.G.                                          */
/***********************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>        //pour le _getch()
#include "Winconsole.h"
#include "Affichage.h"

/***********************************************************************************/
/*                         CONSTANTES D'AFFICHAGE                                  */
/***********************************************************************************/
#define HAUT_ETAGE    4    //hauteur d'affichage pour un étage
#define LARG_ETAGE   10    //hauteur d'affichage pour un ascenseur
#define POS_BOUTONS  33    //coordonnée en X des boutons-étage
#define LONG_FILEMAX 40    //largeur maximale des files d'attente de personnes
#define CH_PERS     225    //code ASCII pour afficher une personne

/***********************************************************************************/
// Fonction d'affichage initial qui affiche tous les titres, menus et objets fixes 
void aff_batiment() {
	int i, j;

	/* Afficher les "noms" des ascenseurs */
	textcolor(LIGHTRED);
	gotoxy(1, 0);  printf("A");
	textcolor(LIGHTBLUE);
	gotoxy(11, 0); printf("B");
	textcolor(YELLOW);
	gotoxy(21, 0); printf("C");
	gotoxy(50, 0); printf("APPUYEZ <ESC> POUR QUITTER");

	/* Afficher le numéro d'étage et un séparateur entre chaque étage */
	for (i = 1; i < NB_ETAGES; i++) {
		gotoxy(POS_BOUTONS - 2, (i - 1) * HAUT_ETAGE + 1);
		printf("%d", NB_ETAGES - i);

		gotoxy(POS_BOUTONS, i * HAUT_ETAGE);
		for (j = 0; j <= 45; j++)
			printf("-");
	}

	/* Affichage de "RC" pour le premier étage */
	gotoxy(POS_BOUTONS - 2, (NB_ETAGES - 1) * HAUT_ETAGE + 1);
	printf("RC");

	/* Affichage du menu-vitesse */
	gotoxy(40, 24); printf("VITESSE: (L)ent, (M)oyen, (R)apide");
}

/***********************************************************************************/
// Afficher le temps avec format h:mm:ss
void aff_temps(int time) {
	/* séparer le temps en heures, minutes et secondes */
	int hr = time / 3600;
	int min = (time % 3600) / 60;
	int sec = time % 60;

	textcolor(WHITE);
	gotoxy(POS_BOUTONS, 0);
	printf("TEMPS : %d:", hr);
	if (min < 10)	printf("0");   //ex. "04"
	printf("%d:", min);
	if (sec < 10)	printf("0");   //ex. "04"
	printf("%d", sec);
}

/***********************************************************************************/
// Dessiner l'ascenseur #no avec le "mode" reçu 
void dessiner_asc(const t_asc* asc, int no, t_mode mode) {
	int etage = get_eta_asc(asc),            //étage actuel de l'ascenseur
		nbper = get_nb_pers(asc);            //nombre de personnes dans l'ascenseur
	int posx = no * LARG_ETAGE + 3, i,
		posy = (NB_ETAGES - etage - 1) * HAUT_ETAGE;

	/* choix de couleur arbitraire pour chaque ascenseur */
	if (no == 0)
		textcolor(LIGHTRED);
	else if (no == 1)
		textcolor(LIGHTBLUE);
	else
		textcolor(YELLOW);

	/* les 3 boucles for suivantes vont déssiner le contour de l'ascenseur */
	gotoxy(posx, posy); 	printf("%c", 201);
	for (i = 1; i <= 5; i++)
		printf("%c", 205);
	printf("%c", 187);

	for (i = 1; i <= 3; i++) {
		gotoxy(posx, posy + i);
		printf("%c     %c", 186, 186);
	}

	gotoxy(posx, posy + 4); printf("%c", 200);
	for (i = 1; i <= 5; i++)
		printf("%c", 205);
	printf("%c", 188);

	/* afficher les 6 boutons-étage dans cet ascenseur */
	for (i = 0; i < NB_ETAGES; i++) {
		if (get_bt_asc(asc, i))
			textcolor(LIGHTRED);      //couleur pour un bouton pesé
		else
			textcolor(LIGHTGRAY);     //bouton éteint

		gotoxy(posx + (i % 2) + 1, posy + (3 - i / 2));
		printf("%c", (i) ? (48 + i) : 'R');       //soit 1-5 ou R (pour i==0)
	}

	/* afficher le "mode" de l'ascenseur avec son étage actuel */
	textcolor(LIGHTRED);
	gotoxy(posx + 4, posy + 1);
	switch (mode) {
	case STOP: printf(">");       break;
	case UP: printf("%c", '^'); break;    //flèche-HAUT
	case DOWN: printf("%c", 'v');           //flèche-BAS
	}
	printf("%c", (etage) ? (48 + etage) : 'R');  //soit 1-5 ou R (pour etage==0)

	/* afficher le nombre de personnes dans cet ascenseur */
	textcolor(WHITE);
	gotoxy(posx + 4, posy + 3);
	if (nbper) printf("%2d", nbper);
	else       printf("  ");           //vide
}

/***********************************************************************************/
// Effacer un ascenseur se trouvant à l'étage "etage" dans la colonne "no" 
void effacer_asc(int etage, int no) {
	int posx = no * LARG_ETAGE + 3, i,                //posx selon le "no" reçu
		posy = (NB_ETAGES - etage - 1) * HAUT_ETAGE;  //posy selon l'"etage"

	/* pour toute la hauteur d'un étage, afficher LARG_ETAGE espaces */
	for (i = 0; i <= HAUT_ETAGE; i++) {
		gotoxy(posx, posy + i);
		printf("       ");
	}
}

/***********************************************************************************/
// Afficher les bouton-étages du bâtiment 
void aff_boutons(t_boutons boutons) {
	int i, posx = POS_BOUTONS;

	/* pour tous les 6 étages.. */
	for (i = 0; i < NB_ETAGES; i++) {
		/* déssiner le bouton-HAUT sauf pour le dernier étage! */
		if (i != NB_ETAGES - 1) {
			gotoxy(posx, 2 + (NB_ETAGES - i - 1) * HAUT_ETAGE);
			if (get_bouton(boutons, i, UP))
				textcolor(LIGHTRED);      //bouton est allumé
			else
				textcolor(LIGHTGRAY);     //bouton éteint
			printf("%c", '^');          //flèche-HAUT
		}

		/* déssiner le bouton-BAS sauf pour le rez-de-chaussée! */
		if (i != RC) {
			gotoxy(posx, 3 + (NB_ETAGES - i - 1) * HAUT_ETAGE);
			if (get_bouton(boutons, i, DOWN))
				textcolor(LIGHTRED);      //bouton est allumé
			else
				textcolor(LIGHTGRAY);     //bouton éteint
			printf("%c", 'v');          //flèche-BAS
		}
	}
}

/***********************************************************************************/
// Afficher la file d'attente de l'"etage" reçu pour la "dir" reçue 
void aff_file(t_file file, int etage, t_mode dir) {
	int i, nb = get_nb_file(file),     //le nombre de personne dans cette file
		posx = POS_BOUTONS + 2,
		posy = 2 + (NB_ETAGES - etage - 1) * HAUT_ETAGE;  //posy selon l"etage"

	textcolor(WHITE);
	gotoxy(posx, (dir == UP) ? posy : posy + 1);   //posy varie selon la direction voulue
	for (i = 0; i < nb; i++)    //pour chaque personne dans la file on dessine,
		printf("%c", CH_PERS);      //on affiche une personne

	for (; i < LONG_FILEMAX; i++)    //effacer restant de cette ligne-écran
		printf(" ");
	textcolor(LIGHTGRAY);
}

/***********************************************************************************/
// Afficher un nombre de personnes "nb" sorties à l"etage" reçu 
void aff_sorties(int nb, int etage) {
	int posx = 76,
		posy = 1 + (NB_ETAGES - etage - 1) * HAUT_ETAGE;   //posy selon l"etage"

	textcolor(LIGHTGRAY);
	gotoxy(posx, posy);
	printf("%3d", nb);     //le nombre de personnes reçu
}

/***********************************************************************************/
// Afficher la moyenne de temps perdu par personne ET faire une pause-écran 
void aff_moyenne(double moy_tmp_perdu) {
	textcolor(WHITE);
	gotoxy(40, 24);
	printf("Moyenne temps perdu: %.2lf secondes ", moy_tmp_perdu);

	_getch();   //pause-écran
}

/***********************************************************************************/
// Faire un délai-écran en millisecondes (encapsule le "delay()" de "Winconsole.h") 
void delai(int msec) {
	delay(msec);
}

/***********************************************************************************/
/***********************************************************************************/