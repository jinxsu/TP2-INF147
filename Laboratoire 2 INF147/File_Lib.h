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
