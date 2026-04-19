/*#include <stdio.h>
#include<assert.h>
#include"File_Lib.h"
#include "t_ascenseur.h"
#include "t_personnes.h"
#include "t_liste_even.h"
#include "t_even.h"

void print_test(const char* test_name, int condition) {
	if (condition) {
		printf("Test %s: PASSED\n", test_name);
	} else {
		printf("Test %s: FAILED\n", test_name);
	}
}

//Test pour t_file
void test_init_file() {
    t_file* file = init_file(5);
    assert(file != NULL);
    assert(file->total == 0);
	assert(file->taille_file == 5);
    assert(file->debut == 0);
    assert(file->fin == 0);
	detruire_file(&file);
	assert(file == NULL);
	print_test("init_file", 1);
}
void test_est_vide() {
	t_file* file = init_file(5);
	assert(est_vide(*file) == 1);
	ajouter_fin(*file, 1);
	assert(est_vide(*file) == 0);
	detruire_file(&file);
	print_test("est_vide", 1);
}
void test_est_pleine() {
	t_file* file = init_file(3);
	assert(est_pleine(*file) == 0);
	ajouter_fin(*file, 1);
	ajouter_fin(*file, 2);
	ajouter_fin(*file, 3);
	assert(est_pleine(*file) == 1);
	detruire_file(&file);
	print_test("est_pleine", 1);
}
void test_ajouter_fin() {
	t_file* file = init_file(3);
	assert(ajouter_fin(*file, 1) == 1);
	assert(ajouter_fin(*file, 2) == 1);
	assert(ajouter_fin(*file, 3) == 1);
	assert(ajouter_fin(*file, 4) == 0); // file should be full
	detruire_file(&file);
	print_test("ajouter_fin", 1);
}
void test_enlever_debut() {
	t_file* file = init_file(3);
	ajouter_fin(*file, 1);
	ajouter_fin(*file, 2);
	ajouter_fin(*file, 3);
	int item;
	assert(enlever_debut(*file, &item) == 1);
	assert(item == 1);
	assert(enlever_debut(*file, &item) == 1);
	assert(item == 2);
	assert(enlever_debut(*file, &item) == 1);
	assert(item == 3);
	assert(enlever_debut(*file, &item) == 0); // file should be empty
	detruire_file(&file);
	print_test("enlever_debut", 1);
}
void test_get_nb_file() {
	t_file* file = init_file(5);
	assert(get_nb_file(*file) == 0);
	ajouter_fin(*file, 1);
	ajouter_fin(*file, 2);
	assert(get_nb_file(*file) == 2);
	enlever_debut(*file, NULL);
	assert(get_nb_file(*file) == 1);
	detruire_file(&file);
	print_test("get_nb_file", 1);
}
void test_file_circulaire() {
	t_file* file = init_file(3);
	ajouter_fin(*file, 1);
	ajouter_fin(*file, 2);
	ajouter_fin(*file, 3);
	enlever_debut(*file, 1);
	int item;
	assert(enlever_debut(*file, &item) == 2);
	assert(item == 1);
	assert(enlever_debut(*file, &item) == 3);
	assert(ajouter_fin(*file, 4) == 1); // should wrap around
	assert(est_pleine(*file) == 1);
	detruire_file(&file);
	print_test("file_circulaire", 1);
}
void run_tests_file() {
	test_init_file();
	test_est_vide();
	test_est_pleine();
	test_ajouter_fin();
	test_enlever_debut();
	test_get_nb_file();
	test_file_circulaire();
}
//Test pour t_even

void test_init_even() {
	t_even e = init_even(BOUTON, 3, 5, 10);
	
}
void test_get_type() {
	t_even e = init_even(BOUTON, 3, 5, 10);
	assert(get_type(&e) == BOUTON);
	print_test("get_type", 1);
}
void test_get_etage() {
	t_even e = init_even(BOUTON, 3, 5, 10);
	assert(get_etage(&e) == 3);
	print_test("get_etage", 1);
}
void test_get_no() {
	t_even e = init_even(BOUTON, 3, 5, 10);
	assert(get_no(&e) == 5);
	print_test("get_no", 1);
}
void test_get_temps() {
	t_even e = init_even(BOUTON, 3, 5, 10);
	assert(get_temps(&e) == 10);
	print_test("get_temps", 1);
}
void run_tests_even() {
	test_init_even();
	test_get_type();
	test_get_etage();
	test_get_no();
	test_get_temps();
}
//Test pour t_ascenseur
void test_init_asc() {
	t_asc asc;
	init_asc(&asc, 1);
	assert(asc.dispo == 1);
	assert(asc.etage == RC);
	assert(asc.mode == STOP);
	assert(asc.modesuiv == STOP);
	assert(asc.nbper == 0);
	for (int i = 0; i < NB_ETAGES-1; i++) {
		assert(asc.boutons[i] == 0);
		assert(est_vide(&asc.files_personnes[i]) == 1);
	}
	detruire_files_asc(&asc, 1);
	print_test("init_asc", 1);
}

void run_tests_asc() {
	test_init_asc();
	
}
*/