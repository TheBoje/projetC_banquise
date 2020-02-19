#ifndef BANQUISE_H_INCLUDED
#define BANQUISE_H_INCLUDED

#include"objet.h"

/* ---------- Types Banquise ---------- */

typedef enum
{
    glace,
    eau,
    vide
} T_type_case;


typedef enum
{
    depart,
    arrive,
    defaut
} T_but;

typedef struct
{
    T_type_case type_case;
    T_but but;
    T_pos position;
    T_objet objet;
    T_joueur* joueur;
} T_case;

typedef struct
{
    T_case **tab;
    int taille;
    int nombre_joueur;
} T_banquise;

#endif // BANQUISE_H_INCLUDED
