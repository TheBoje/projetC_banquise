#ifndef BANQUISE_H_INCLUDED
#define BANQUISE_H_INCLUDED

#include"objet.h"

/* ---------- Types Banquise ---------- */
/*
    Definition des differents objets :
    - T_type_case (enum)
    - T_but (enum)
    - T_taille (int tailleX, int tailleY)
    - T_case (T_type_case type_case, T_but but, T_objet objet, T_joueur* joueur, T_vec vecteur)
    - T_banquise (T_case **tab, int taille, int nombre_joueur)
*/


typedef enum // Definition du T_type_case, enumere les possibilites d'une case de la banquise
{
    glace = 0,
    eau = 1
} T_type_case;


typedef enum // Definition du T_but, enumere les differentes cases speciales. C'est a dire la case de depart et la case d'arrivee.
{
    depart = 0,
    arrive = 1,
    defaut
} T_but;


typedef struct // Definition du T_case, regroupe toutes les proprietes d'une case.
{
    T_type_case type_case;
    T_but but;
    T_objet objet;
    T_joueur *joueur;
    T_vec vecteur;
} T_case;


typedef struct // Definition du T_banquise, contient la matrice de cases, la taille de la matrice et le nombre de joueur
{
    T_case **tab;
    int taille;
    int nombre_joueur;
} T_banquise;

/* ---------- Fonctions Banquise ---------- */

/* Code Louis */
void init_random();

void remp_banquise_tab_aux(T_case **tab, int taille, int x, int y);

T_case **create_tab(int taille);

T_banquise create_banquise(int taille, int joueurs);

void modif_type_case(T_banquise banquise, int x, int y, T_type_case type_case);

void modif_pos_joueur(T_banquise banquise, T_joueur joueur);

T_pos position_arrive (T_banquise banquise);

int **create_tab_chemin(int taille);

int **init_chemin_existe(T_banquise banquise, T_pos pos, int taille);

int chemin_existe(T_banquise banquise, T_pos pos, int **search);

char T_but_to_char(T_but objet);

char T_case_to_char(T_type_case c);

/* Code Ines */

void remp_banquise_tab(T_case **tab, int n);

T_case **alloue(int n);

void choisir_case_arrive(T_case **tab, int taille);

void choisir_case_depart(T_case **tab, int taille);


#endif // BANQUISE_H_INCLUDED
