#ifndef BANQUISE_H_INCLUDED
#define BANQUISE_H_INCLUDED

#include"objet.h"

/* ---------- Types Banquise ---------- */
/*
    Definition des differents objets :
    - T_type_case (enumere)
    - T_but (enumere)
    - T_taille (int tailleX, int tailleY)
    - T_case (T_type_case type_case, T_but but, T_pos position, T_objet objet, T_joueur* joueur)
    - T_banquise (T_case **tab, int taille, int nombre_joueur)
*/


typedef enum // Definition du T_type_case, enumere les possibilites d'une case de la banquise
{
    glace,
    eau,
    vide
} T_type_case;


typedef enum // Definition du T_but, enumere les differentes cases speciales. C'est a dire la case de depart et la case d'arrivee.
{
    depart,
    arrive,
    defaut
} T_but;

typedef struct {
    int tailleX;
    int tailleY;
} T_taille;


typedef struct // Definition du T_case, regroupe toutes les proprietes d'une case.
{
    T_type_case type_case;
    T_but but;
    T_pos position;
    T_objet objet;
    T_joueur* joueur;
} T_case;


typedef struct // Definition du T_banquise, contient la matrice de cases, la taille de la matrice et le nombre de joueur
{
    T_case **tab;
    T_taille taille;
    int nombre_joueur;
} T_banquise;

#endif // BANQUISE_H_INCLUDED
