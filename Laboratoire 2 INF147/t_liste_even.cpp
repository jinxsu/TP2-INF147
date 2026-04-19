/*****************************************************************************/
/*	T_LISTE_EVEN.cpp									   				     */
/*																		     */
/*	Description: Module qui implémente une liste chaînée pour les événements */
/*	Conception : Zhang Rui Chen et Samuel Dero							     */
/*****************************************************************************/

#include <stdlib.h>
#include "t_liste_even.h"

t_liste creer_liste(void)
{
    t_liste nouv_liste = (t_liste)malloc(sizeof(struct liste));
    nouv_liste->taille = 0;
    nouv_liste->tete = NULL;
    nouv_liste->fin = NULL;
    return nouv_liste;
}

void vider_liste(t_liste liste)
{
    t_pointeur current = liste->tete;
    while (current != NULL) {
        t_pointeur suivant = current->prochain;
        free(current);
        current = suivant;
    }
    liste->taille = 0;
    liste->tete = NULL;
    liste->fin = NULL;
}

void detruire_liste(t_liste* pliste)
{
    vider_liste(*pliste);
    free(*pliste);
    *pliste = NULL;
}

int inserer_element(t_liste liste, t_type_even even, int eta, int no, int temps)
{
    t_pointeur nouveau = (t_pointeur)malloc(sizeof(struct noeud));
    if (nouveau == NULL)
        return 0;

    nouveau->data = init_even(even, eta, no, temps);
    nouveau->prochain = NULL;

    if (liste->taille == 0) {
        liste->tete = nouveau;
        liste->fin = nouveau;
    }
    else if (temps >= liste->fin->data.temps) {
        liste->fin->prochain = nouveau;
        liste->fin = nouveau;
    }
    else {
        t_pointeur current = liste->tete;
        t_pointeur previous = NULL;
        while (current != NULL && current->data.temps <= temps) {
            previous = current;
            current = current->prochain;
        }
        if (previous == NULL) {
            nouveau->prochain = liste->tete;
            liste->tete = nouveau;
        }
        else {
            previous->prochain = nouveau;
            nouveau->prochain = current;
        }
    }
    liste->taille++;
    return 1;
}

void lire_premier_element(const t_liste liste, t_even* evenement)
{
    if (liste->tete != NULL)
        *evenement = liste->tete->data;
}

void retirer_premier_element(t_liste liste)
{
    if (liste->tete == NULL)
        return;
    t_pointeur temp = liste->tete;
    liste->tete = liste->tete->prochain;
    if (liste->tete == NULL)
        liste->fin = NULL;
    free(temp);
    liste->taille--;
}

int lire_taille_liste(const t_liste liste)
{
    return liste->taille;
}