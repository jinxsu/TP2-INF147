#pragma once
/***********************************************************************************/
/*  AFFICHAGE.h - Module d'affichage                                               */
/*                                                                                 */
/*  Ce module va d�finir toutes les fonctionnalit�s d'affichage du syst�me         */
/*  d'ascenseurs incluant l'�tat des boutons, les files d'attentes et le nombre    */
/*  de personnes sortis � chaque �tage.                                            */
/*  Conception : E.Th�, D.E.G.                                                     */
/***********************************************************************************/
#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__

#include "t_ascenseur.h"

/* Fonction d'affichage initial qui affiche tous les titres, menus et objets fixes */
void aff_batiment();

/* Afficher le temps avec format h:mm:ss */
void aff_temps(int time);

/* Dessiner l'ascenseur #no avec le "mode" re�u */
void dessiner_asc(const t_asc* asc, int no, t_mode mode);

/* Effacer un ascenseur se trouvant � l'�tage "etage" dans la colonne "no" */
void effacer_asc(int etage, int no);

/* Afficher les bouton-�tages du b�timent */
void aff_boutons(t_boutons boutons);

/* Afficher la file d'attente de l'"etage" re�u pour la "dir" re�ue */
void aff_file(t_file file, int etage, t_mode dir);

/* Afficher un nombre de personnes "nb" sorties � l"etage" re�u */
void aff_sorties(int nb, int etage);

/* Afficher la moyenne de temps perdu par personne ET faire une pause-�cran */
void aff_moyenne(double moy_tmp_perdu);

/* Faire un d�lai-�cran en millisecondes (remplace le "delay()" de "Winconsole.h") */
void delai(int msec);

#endif
