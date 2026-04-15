#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>

#include "File_Lib.h"
#include "t_even.h"
#include "t_liste_even.h"
#include "t_personnes.h"
#include "t_ascenseur.h"
#include "Affichage.h"

/***********************************************************************************/
/* Constantes globales de la simulation                                            */
/***********************************************************************************/
#define NB_ASC      3     /* 3 ascenseurs */
#define TMP_DEPL    2     /* 2 sec = temps de deplacement d'un etage a un autre */
#define TMP_ARRET   5     /* 5 sec = duree normale d'arret a un etage */
#define NB_PERS  1000     /* nb. total de personnes */
#define MAX_FILE   40     /* taille des files d'attente a chaque etage */
#define TMP_ARRMAX 20     /* 20 sec = intervalle maximal entre 2 arrivees */
#define TMP_TOT 10800     /* 10800 sec = 3 heures max de simulation */

/* Probabilites d'arrivee a un etage donne (aussi utilisee pour l'etage-dest
   quand la personne n'est PAS au RC) */
const double PROB_ETAG[NB_ETAGES] = { 0.5, 0.1, 0.1, 0.1, 0.1, 0.1 };

/* Probabilites pour l'etage-destination quand la personne est au RC */
const double PROB_DEST[NB_ETAGES] = { 0, 0.05, 0.2, 0.3, 0.3, 0.15 };

/***********************************************************************************/
/* Prototypes des sous-fonctions de la simulation                                  */
/***********************************************************************************/
void traitement_even_bouton(t_even even, t_personne liste_pers[],
    t_files_att files_att[], t_boutons boutons);

void traitement_even_depart(t_even even, int temps,
    t_asc liste_asc[], t_liste liste_evenements);

void traitement_even_portes(t_even even, int temps,
    t_asc liste_asc[], t_boutons boutons,
    t_personne liste_pers[], t_files_att files_att[],
    t_liste liste_evenements, int nb_sortis[]);

void traitement_even_arrivee(t_even even, int temps,
    t_asc liste_asc[], t_boutons boutons,
    t_liste liste_evenements);

void verifier_boutons_etage(int temps, t_boutons boutons,
    t_asc liste_asc[], t_liste liste_evenements);

/***********************************************************************************/
/* Prototypes des tests unitaires                                                  */
/***********************************************************************************/
void print_test(const char* test_name, int condition);
void run_tests_file(void);
void run_tests_even(void);
void run_tests_asc(void);

/***********************************************************************************/
/* main                                                                            */
/***********************************************************************************/
int main(void)
{
 

    /* --- Initialisation de la simulation --- */
    t_liste      liste_evenements;
    t_personne* liste_pers;
    t_asc        liste_asc[NB_ASC];
    t_boutons    boutons;
    t_files_att  files_att[NB_ETAGES];   /* 2 files par etage (UP/DN) */
    int          nb_sortis[NB_ETAGES];   /* personnes sorties par etage */
    int          tmp_delai = 50;         /* vitesse moyenne par defaut */
    FILE* logfile;

    /* Ouvrir le fichier log */
    logfile = fopen("simulation.log", "w");

    /* Creer la liste d'evenements */
    liste_evenements = creer_liste();

    /* Initialiser les files d'attente aux etages */
    for (int i = 0; i < NB_ETAGES; i++) {
        files_att[i].file_up = init_file(MAX_FILE);
        files_att[i].file_dn = init_file(MAX_FILE);
        nb_sortis[i] = 0;
    }

    /* Initialiser les boutons-etages */
    init_boutons(boutons);

    /* Initialiser les ascenseurs */
    init_asc(liste_asc, NB_ASC);

    /* Generer toutes les personnes et leurs evenements BOUTON */
    liste_pers = init_personnes(NB_PERS, liste_evenements, TMP_ARRMAX,
        PROB_ETAG, PROB_DEST, NB_ETAGES);

    /* Affichage initial du batiment */
    aff_batiment();
    for (int i = 0; i < NB_ASC; i++)
        dessiner_asc(&liste_asc[i], i, STOP);
    aff_boutons(boutons);

    /* --- Boucle principale de simulation --- */
    while (lire_taille_liste(liste_evenements) > 0) {

        /* Lire et retirer le prochain evenement chronologique */
        t_even even;
        lire_premier_element(liste_evenements, &even);
        retirer_premier_element(liste_evenements);

        int temps = get_temps(&even);

        /* Arreter la simulation si on depasse le temps maximal */
        if (temps > TMP_TOT)
            break;

        /* Afficher le temps courant */
        aff_temps(temps);

        /* Ecrire dans le fichier log */
        if (logfile) {
            const char* nom_even[] = { "BOUTON ", "DEPART ", "PORTES ", "ARRIVEE" };
            fprintf(logfile, "%s: Etage %d, No. %d, Temps: %d\n",
                nom_even[get_type(&even)],
                get_etage(&even),
                get_no(&even),
                temps);
        }

        /* Traiter l'evenement selon son type */
        switch (get_type(&even)) {
        case BOUTON:
            traitement_even_bouton(even, liste_pers, files_att, boutons);
            verifier_boutons_etage(temps, boutons, liste_asc, liste_evenements);
            break;
        case DEPART:
            traitement_even_depart(even, temps, liste_asc, liste_evenements);
            break;
        case PORTES:
            traitement_even_portes(even, temps, liste_asc, boutons,
                liste_pers, files_att, liste_evenements, nb_sortis);
            verifier_boutons_etage(temps, boutons, liste_asc, liste_evenements);
            break;
        case ARRIVEE:
            traitement_even_arrivee(even, temps, liste_asc, boutons, liste_evenements);
            break;
        }

        /* Gestion du clavier et du delai d'affichage */
        if (_kbhit()) {
            int touche = _getch();
            if (touche == 27)   /* ESC */
                break;
            else if (touche == 'L' || touche == 'l') tmp_delai = 500;
            else if (touche == 'M' || touche == 'm') tmp_delai = 50;
            else if (touche == 'R' || touche == 'r') tmp_delai = 5;
        }
        delai(tmp_delai);
    }

    /* --- Fin de simulation --- */
    double moy = calculer_tmp_perdu(liste_pers, NB_PERS);
    aff_moyenne(moy);

    /* Nettoyage */
    if (logfile) fclose(logfile);

    for (int i = 0; i < NB_ETAGES; i++) {
        detruire_file(&files_att[i].file_up);
        detruire_file(&files_att[i].file_dn);
    }
    detruire_files_asc(liste_asc, NB_ASC);
    detruire_liste_pers(&liste_pers);
    detruire_liste(&liste_evenements);

    return 0;
}

/***********************************************************************************/
/* A. traitement_even_bouton                                                       */
/* Ajoute la personne dans la bonne file d'attente de son etage et allume le       */
/* bouton correspondant.                                                            */
/***********************************************************************************/
void traitement_even_bouton(t_even even, t_personne liste_pers[],
    t_files_att files_att[], t_boutons boutons)
{
    int no_pers = get_no(&even);
    int etage_arr = get_etage_arr(&liste_pers[no_pers]);
    int etage_dst = get_etage_dest(&liste_pers[no_pers]);

    if (etage_dst > etage_arr) {
        /* La personne veut monter */
        ajouter_fin(files_att[etage_arr].file_up, no_pers);
        aff_file(files_att[etage_arr].file_up, etage_arr, UP);
        set_bouton_up(&boutons[etage_arr]);
    }
    else {
        /* La personne veut descendre */
        ajouter_fin(files_att[etage_arr].file_dn, no_pers);
        aff_file(files_att[etage_arr].file_dn, etage_arr, DOWN);
        set_bouton_dn(&boutons[etage_arr]);
    }
    aff_boutons(boutons);
}

/***********************************************************************************/
/* B. traitement_even_depart                                                       */
/* Transfert le modesuiv dans mode, calcule le prochain etage et insere un         */
/* evenement ARRIVEE. Efface visuellement l'ascenseur de son etage actuel.         */
/***********************************************************************************/
void traitement_even_depart(t_even even, int temps,
    t_asc liste_asc[], t_liste liste_evenements)
{
    int    no_asc = get_no(&even);
    t_asc* asc = &liste_asc[no_asc];
    int    etage = get_eta_asc(asc);

    /* Copier modesuiv dans mode */
    transfert_modesuiv(asc);

    /* Calculer le prochain etage selon la direction */
    int prochain_etage;
    if (asc->mode == UP)
        prochain_etage = etage + 1;
    else
        prochain_etage = etage - 1;

    /* Inserer l'evenement ARRIVEE dans la liste */
    inserer_element(liste_evenements, ARRIVEE, prochain_etage, no_asc,
        temps + TMP_DEPL);

    /* Effacer visuellement l'ascenseur a son etage actuel */
    effacer_asc(etage, no_asc);
}

/***********************************************************************************/
/* C. traitement_even_portes                                                       */
/* Ouvre les portes : fait sortir les gens, fait entrer les gens en attente,       */
/* puis decide si on repart (DEPART) ou si on garde les portes ouvertes (PORTES).  */
/***********************************************************************************/
void traitement_even_portes(t_even even, int temps,
    t_asc liste_asc[], t_boutons boutons,
    t_personne liste_pers[], t_files_att files_att[],
    t_liste liste_evenements, int nb_sortis[])
{
    int    no_asc = get_no(&even);
    t_asc* asc = &liste_asc[no_asc];
    int    etage = get_eta_asc(asc);

    /* Afficher l'ascenseur en mode STOP */
    dessiner_asc(asc, no_asc, STOP);

    /* 1. Faire sortir les gens qui veulent descendre a cet etage */
    int nb_sortis_ici = vider_file_asc(asc, etage, liste_pers, temps);
    nb_sortis[etage] += nb_sortis_ici;

    /* 2. Faire entrer les gens selon la direction de l'ascenseur (modesuiv) */
    int nb_entres = 0;
    t_file file_entree = NULL;

    if (asc->modesuiv == UP) {
        file_entree = files_att[etage].file_up;
        nb_entres = transfert_file(asc, file_entree, liste_pers);
        aff_file(files_att[etage].file_up, etage, UP);
        /* Eteindre le bouton UP si la file est vide */
        if (est_vide(files_att[etage].file_up))
            clr_bouton_up(&boutons[etage]);
    }
    else if (asc->modesuiv == DOWN) {
        file_entree = files_att[etage].file_dn;
        nb_entres = transfert_file(asc, file_entree, liste_pers);
        aff_file(files_att[etage].file_dn, etage, DOWN);
        /* Eteindre le bouton DN si la file est vide */
        if (est_vide(files_att[etage].file_dn))
            clr_bouton_dn(&boutons[etage]);
    }

    aff_boutons(boutons);

    /* Redessiner l'ascenseur et afficher le total des sorties */
    dessiner_asc(asc, no_asc, asc->modesuiv);
    aff_sorties(nb_sortis[etage], etage);

    /* 3. Si l'ascenseur est completement vide -> le remettre disponible */
    if (asc->nbper == 0 && est_vide(files_att[etage].file_up)
        && est_vide(files_att[etage].file_dn)) {
        set_dispo(asc, 1);
    }

    /* 4. Decider : DEPART ou garder les PORTES ouvertes ? */
    int total_transfert = nb_sortis_ici + nb_entres;

    if (total_transfert < CAPACITE / 2) {
        /* Peu de monde : depart normal */
        inserer_element(liste_evenements, DEPART, etage, no_asc,
            temps + TMP_ARRET);
    }
    else {
        /* Beaucoup de monde : garder les portes ouvertes plus longtemps */
        inserer_element(liste_evenements, PORTES, etage, no_asc,
            temps + TMP_ARRET);
    }
}

/***********************************************************************************/
/* D. traitement_even_arrivee                                                      */
/* L'ascenseur arrive a un etage. On choisit son nouveau mode via choix_mode().    */
/* Si on s'arrete : inserer PORTES. Sinon : inserer ARRIVEE au prochain etage.     */
/***********************************************************************************/
void traitement_even_arrivee(t_even even, int temps,
    t_asc liste_asc[], t_boutons boutons,
    t_liste liste_evenements)
{
    int    no_asc = get_no(&even);
    int    etage_dest = get_etage(&even);
    t_asc* asc = &liste_asc[no_asc];

    /* Mettre a jour l'etage actuel et copier mode dans modesuiv */
    set_eta_asc(asc, etage_dest);
    transfert_mode(asc);   /* .mode --> .modesuiv */

    /* Choisir le nouveau mode actuel de l'ascenseur */
    t_mode nouveau_mode = choix_mode(asc, boutons, etage_dest);

    /* Afficher l'ascenseur avec le nouveau mode */
    dessiner_asc(asc, no_asc, nouveau_mode);

    if (nouveau_mode == STOP) {
        /* S'arreter a cet etage : ouvrir les portes */
        inserer_element(liste_evenements, PORTES, etage_dest, no_asc,
            temps + TMP_ARRET);
    }
    else {
        /* Continuer sans s'arreter */
        int prochain_etage = (nouveau_mode == UP) ? etage_dest + 1 : etage_dest - 1;

        inserer_element(liste_evenements, ARRIVEE, prochain_etage, no_asc,
            temps + TMP_DEPL);

        /* Effacer l'ascenseur apres un petit delai */
        delai(30);
        effacer_asc(etage_dest, no_asc);
    }
}

/***********************************************************************************/
/* E. verifier_boutons_etage                                                       */
/* Parcourt tous les etages (du plus haut au RC). Pour chaque bouton pese,         */
/* essaie d'appeler un ascenseur disponible et insere l'evenement approprie.       */
/***********************************************************************************/
void verifier_boutons_etage(int temps, t_boutons boutons,
    t_asc liste_asc[], t_liste liste_evenements)
{
    if (!bouton_en_attente(boutons))
        return;

    /* Parcourir du dernier etage jusqu'au RC */
    for (int i = NB_ETAGES - 1; i >= RC; i--) {
        if (boutons[i].up || boutons[i].down) {

            int no_asc = appelle_asc(liste_asc, NB_ASC, i, boutons);

            if (no_asc != AUCUN) {
                /* Mettre l'ascenseur non-disponible */
                set_dispo(&liste_asc[no_asc], 0);

                int etage_asc = get_eta_asc(&liste_asc[no_asc]);

                if (etage_asc == i) {
                    /* L'ascenseur est deja a cet etage : ouvrir les portes */
                    inserer_element(liste_evenements, PORTES, i, no_asc,
                        temps + TMP_DEPL);
                }
                else {
                    /* L'ascenseur doit se deplacer : inserer un DEPART */
                    inserer_element(liste_evenements, DEPART, etage_asc, no_asc,
                        temps + TMP_DEPL);
                }
            }
        }
    }
}

/***********************************************************************************/
/* Tests unitaires                                                                 */
/***********************************************************************************/
void print_test(const char* test_name, int condition)
{
    printf("Test %-25s : %s\n", test_name, condition ? "PASSED" : "FAILED");
}

/* --- Tests File --- */
static void test_init_file(void)
{
    t_file f = init_file(5);
    assert(f != NULL);
    assert(f->total == 0);
    assert(f->taille_file == 5);
    assert(f->debut == 0);
    assert(f->fin == 0);
    detruire_file(&f);
    assert(f == NULL);
    print_test("init_file", 1);
}

static void test_est_vide(void)
{
    t_file f = init_file(5);
    assert(est_vide(f) == 1);
    ajouter_fin(f, 1);
    assert(est_vide(f) == 0);
    detruire_file(&f);
    print_test("est_vide", 1);
}

static void test_est_pleine(void)
{
    t_file f = init_file(3);
    assert(est_pleine(f) == 0);
    ajouter_fin(f, 1);
    ajouter_fin(f, 2);
    ajouter_fin(f, 3);
    assert(est_pleine(f) == 1);
    detruire_file(&f);
    print_test("est_pleine", 1);
}

static void test_ajouter_fin(void)
{
    t_file f = init_file(3);
    assert(ajouter_fin(f, 1) == 1);
    assert(ajouter_fin(f, 2) == 1);
    assert(ajouter_fin(f, 3) == 1);
    assert(ajouter_fin(f, 4) == 0);  /* pleine */
    detruire_file(&f);
    print_test("ajouter_fin", 1);
}

static void test_enlever_debut(void)
{
    t_file f = init_file(3);
    ajouter_fin(f, 1);
    ajouter_fin(f, 2);
    ajouter_fin(f, 3);
    int item;
    assert(enlever_debut(f, &item) == 1); assert(item == 1);
    assert(enlever_debut(f, &item) == 1); assert(item == 2);
    assert(enlever_debut(f, &item) == 1); assert(item == 3);
    assert(enlever_debut(f, &item) == 0); /* vide */
    detruire_file(&f);
    print_test("enlever_debut", 1);
}

static void test_get_nb_file(void)
{
    t_file f = init_file(5);
    assert(get_nb_file(f) == 0);
    ajouter_fin(f, 1);
    ajouter_fin(f, 2);
    assert(get_nb_file(f) == 2);
    int item;
    enlever_debut(f, &item);
    assert(get_nb_file(f) == 1);
    detruire_file(&f);
    print_test("get_nb_file", 1);
}

static void test_file_circulaire(void)
{
    t_file f = init_file(3);
    ajouter_fin(f, 1);
    ajouter_fin(f, 2);
    ajouter_fin(f, 3);
    int item;
    enlever_debut(f, &item);  /* retire 1 */
    assert(ajouter_fin(f, 4) == 1); /* wrap-around */
    assert(get_nb_file(f) == 3);
    enlever_debut(f, &item); assert(item == 2);
    enlever_debut(f, &item); assert(item == 3);
    enlever_debut(f, &item); assert(item == 4);
    detruire_file(&f);
    print_test("file_circulaire", 1);
}

void run_tests_file(void)
{
    printf("\n=== Tests File_Lib ===\n");
    test_init_file();
    test_est_vide();
    test_est_pleine();
    test_ajouter_fin();
    test_enlever_debut();
    test_get_nb_file();
    test_file_circulaire();
}

/* --- Tests t_even --- */
void run_tests_even(void)
{
    printf("\n=== Tests t_even ===\n");
    t_even e = init_even(BOUTON, 3, 5, 10);
    assert(get_type(&e) == BOUTON); print_test("get_type", 1);
    assert(get_etage(&e) == 3);      print_test("get_etage", 1);
    assert(get_no(&e) == 5);      print_test("get_no", 1);
    assert(get_temps(&e) == 10);     print_test("get_temps", 1);
}

/* --- Tests t_ascenseur --- */
static void test_init_asc(void)
{
    t_asc asc[1];
    init_asc(asc, 1);
    assert(asc[0].dispo == 1);
    assert(asc[0].etage == RC);
    assert(asc[0].mode == STOP);
    assert(asc[0].modesuiv == STOP);
    assert(asc[0].nbper == 0);
    for (int i = 0; i < NB_ETAGES; i++) {
        assert(asc[0].boutons[i] == 0);
        assert(est_vide(asc[0].files_personnes[i]) == 1);
    }
    detruire_files_asc(asc, 1);
    print_test("init_asc", 1);
}

void run_tests_asc(void)
{
    printf("\n=== Tests t_ascenseur ===\n");
    test_init_asc();
}