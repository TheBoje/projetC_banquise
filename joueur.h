#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED



/* ---------- Types Joueur ---------- */
/*
    Definition des differents types pour le joueur
    - T_couleur (enum)
    - T_pos (int posx, int posy)
    - T_vec (int dx, int dy)
    - T_score (int distance, int nb_glacon, int nb_victime)
    - T_joueur (char nom, T_couleur couleur, int id, T_pos position, T_vec vecteur, T_score score)
*/

typedef enum // Definition du T_couleur, donne la couleur du joueur
{
    BLEU = 0,
    ROUGE,
    VERT,
    JAUNE
} T_couleur;


typedef struct // Definition du T_pos, donne une position dans la matrice de la banquise
{
    int posx;
    int posy;
} T_pos;


typedef struct // Definition du T_vec, donne un vecteur associe a un mouvement
{
    int dx;
    int dy;
} T_vec;


typedef struct // Definition du T_score, regroupe le score du joueur
{
    int distance;
    int nb_glacon;
    int nb_victime;
} T_score;


typedef struct // Definition du T_joueur, regroupe les caracteristiques du joueur
{
    char nom[50];
    T_couleur couleur;
    int id;
    T_pos position;
    T_vec vecteur;
    T_score score;
} T_joueur;



/* Code Louis */

/* Code Ines */

#endif // JOUEUR_H_INCLUDED
