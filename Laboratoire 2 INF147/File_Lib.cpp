#include "File_Lib.h"

t_file init_file(int taille)
{
	t_file f = (t_file)malloc(sizeof(file));
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
