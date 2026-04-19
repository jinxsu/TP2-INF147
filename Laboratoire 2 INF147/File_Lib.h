<<<<<<< HEAD
#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int* items; 
	int debut;
	int fin;
	int total;
	int taille_file;
} file;

typedef file* t_file;

typedef struct {
	t_file file_up;
	t_file file_dn;
} t_files_att;

t_file init_file(int taille);
void detruire_file(t_file* file);
int est_pleine(const t_file file);
int est_vide(const t_file file);
int get_nb_file(const t_file file);
int ajouter_fin(t_file file, int item);
int enlever_debut(t_file file, int *item);
=======
//File_Lib header file



#ifndef FILE_LIB_H
#define FILE_LIB_H

struct file {
    int* items;		 //tableau dynamique des positions-tableau
    int  debut, fin, 	 //positions du début et de fin de la file
        total, 	       //nombre d’éléments actuel
        taille_file;      //la capacité totale de la file
};
typedef struct file* t_file;  //déclaration du type-POINTEUR sur une structure de file

/* Type pour les 2 files d'attentes à un étage (non-encapsulé) */
typedef struct {
    t_file  file_up, file_dn;    //files des gens qui veulent monter et qui veulent descendre
} t_files_att;


// Fonction pour créer une file, la nouvelle file retournée sera vide après un appel de ce contructeur. 
// Le tableau des éléments dans votre « struct file » est un tableau dynamique et sera crée dans cette // fonction selon la « taille » reçue.  Il faudra mémoriser cette taille dans le champ « .taille_file » 
// de la structure.
t_file  initfile(int taille);
void detruirefile(t_file* f);    //Fonction pour détruire la file et remettre le pointeur à NULL
int  est_pleine(const t_file f);    //Vérifie si une file est pleine, retourne 1 si elle est pleine
int  est_vide(const t_file f);      //Vérifie si une file est vide, retourne 1 si elle est vide
int  get_nb_file(const t_file f);   //Retourne le nombre d'éléments dans la file
int  ajouterfin(t_file f, int no);     //Fonction qui ajoute un élément à la fin de la file
int  enleverdebut(t_file f, int* no);  //Fonction qui enleve un élément du début de la file



#endif


>>>>>>> origin/Sam
