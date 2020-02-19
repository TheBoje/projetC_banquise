#ifndef OBJET_H_INCLUDED
#define OBJET_H_INCLUDED

#include"joueur.h"

/* ---------- Types Objet ---------- */
/*
    Definition des differents objets :
    - T_glacon (T_pos position, T_vec vecteur)
    - T_objet (enumere)
*/


typedef struct // Definition du T_glacon, qui possede une position et une direction de mouvement (vecteur)
{
    T_pos position;
    T_vec vecteur;
} T_glacon;


typedef enum // Definition du T_objet, enumere tous les objets presents dans le jeu
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
