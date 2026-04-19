#pragma once
/************************************************************************************/
/*  FILE_LIB.h -  Librairie qui gère les files d'attente circulaires (FIFO)         */
/*                                                                                  */
/*  Conception : Zhang Rui Chen et Samuel Dero                                      */
/************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

struct file {
    int* items;       // tableau dynamique des positions-tableau
    int  debut, fin,  // positions du début et de fin de la file
        total,        // nombre d’éléments actuel
        taille_file;  // la capacité totale de la file
};

// déclaration du type-POINTEUR sur une structure de file
typedef struct file* t_file;

// Type pour les 2 files d'attentes à un étage (non-encapsulé)
typedef struct {
    t_file  file_up, file_dn; 
} t_files_att;


// Function prototypes...
t_file init_file(int taille);
void detruire_file(t_file* file);
int est_pleine(const t_file file);
int est_vide(const t_file file);
int get_nb_file(const t_file file);
int ajouter_fin(t_file file, int item);
int enlever_debut(t_file file, int *item);



