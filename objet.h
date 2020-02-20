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
    glacon = 0,
    resort = 1,
    marteau_tete = 2,
    marteau_manche = 3,
    rocher = 4,
    piege = 5,
    vide = 6
} T_objet;


/* Code Louis */

char  T_objet_to_char(T_objet objet);

/* Code Ines */


#endif // OBJET_H_INCLUDED
