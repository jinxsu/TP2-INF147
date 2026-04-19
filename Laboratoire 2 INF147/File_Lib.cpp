/**************************************************************************/
/*	FILE_LIB.cpp									   				      */
/*																		  */
/*	Description: Librairie qui implémente une file circulaire (FIFO) pour */
/*               les entiers en utilisant l'allocation dynamique.         */
/*               Supporte les opérations standard de file telles que      */
/*               l'ajout (ajouter_fin), le retrait (enlever_debut), et    */
/*               les vérifications de capacité (est_vide, est_pleine).    */
/*	Conception : Zhang Rui Chen et Samuel Dero							  */
/**************************************************************************/

#include "File_Lib.h"

t_file init_file(int taille)
{
    // Ensure the definition of 'struct file' is visible here
    t_file f = (t_file)malloc(sizeof(struct file));
    f->items = (int*)malloc(taille * sizeof(int));
    f->debut = 0;
    f->fin = 0;
    f->total = 0;
    f->taille_file = taille;
    return f;
}

void detruire_file(t_file* file)
{
	free((*file)->items);
	free(*file);
	*file = NULL;
}

int est_pleine(const t_file file)
{
	if (file->total == file->taille_file) { 
		return 1;
	}
	return 0;
}

int est_vide(const t_file file)
{
	if(file->total == 0) {
		return 1;
	}
	return 0;
}

int get_nb_file(const t_file file)
{
	return file->total;
}

int ajouter_fin(t_file file, int item)
{
	if(est_pleine(file)) {
		return 0;
	}
	file->items[file->fin] = item;
	file->fin = (file->fin + 1) % file->taille_file;
	file->total++;
	return 1;
}

int enlever_debut(t_file file, int* item)
{
	if(est_vide(file)) {
		return 0;
	}
	*item = file->items[file->debut];
	file->debut = (file->debut + 1) % file->taille_file;
	file->total--;
	return 1;
}
