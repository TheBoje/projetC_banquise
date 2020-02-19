#ifndef OBJET_H_INCLUDED
#define OBJET_H_INCLUDED

#include"joueur.h"


/* ---------- Types Objet ---------- */

typedef struct
{
    T_pos position;
    T_vec vecteur;
} T_glacon;

typedef enum
{
    glacon,
    resort,
    marteau_tete,
    marteau_manche,
    rocher,
    piege,
    vide
} T_objet;


#endif // OBJET_H_INCLUDED
