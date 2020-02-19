#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

/* ---------- Types Joueur ---------- */
typedef enum
{
    BLEU,
    ROUGE,
    VERT,
    JAUNE
} T_couleur;

typedef struct
{
    int posx;
    int posy;
} T_pos;

typedef struct
{
    int dx;
    int dy;
} T_vec;

typedef struct
{
    int distance;
    int nb_glacon;
    int nb_victime;
} T_score;

typedef struct
{
    char nom[50];
    T_couleur couleur
    int id;
    T_pos position;
    T_vec vecteur;
    T_score score;
} T_joueur;

#endif // JOUEUR_H_INCLUDED
