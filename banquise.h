#ifndef BANQUISE_H_INCLUDED
#define BANQUISE_H_INCLUDED

#include <stdbool.h>
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
    eau = 0,
    glace = 1
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
} T_case;


typedef struct // Definition du T_banquise, contient la matrice de cases, la taille de la matrice et le nombre de joueur
{
    T_case **tab;
    int taille;
    int nombre_joueur;
    int tours;
    T_joueur *joueurs;
} T_banquise;

typedef struct {
    int nbJoueurs;
    char **nomJoueurs;
    T_couleur *couleursJoueurs;
    int mapTaille;
    int statusMenu;
    bool param;
} T_init_jeu;

/* ---------- Fonctions Banquise ---------- */

/* Code Louis */

void init_random();

void print_banquise_game();

void affiche_banquise(T_banquise banquise);

T_vec char_to_t_vec(char c);

void remp_banquise_tab_aux(T_case **tab, int taille, int x, int y);

T_case **create_tab(int taille);

T_banquise create_banquise(int taille, int joueurs);

T_pos position_arrive(T_banquise banquise);

T_pos position_depart(T_banquise banquise);

T_pos offset_pos(T_pos pos, int offx, int offy);

int is_in_banquise(T_banquise banquise, T_pos pos);

void print_search(int **search, T_banquise banquise);

int **create_tab_chemin(int taille);

int **tab_chemin_fill_eau(T_banquise banquise, int **search);

int chemin_exist(T_banquise banquise, T_pos pos);

int chemin_exist_aux(T_banquise banquise, T_pos pos, T_pos pos_arrive, int **search);

char T_but_to_char(T_but objet);

char T_case_to_char(T_type_case c);

T_pos joueur_position(T_banquise banquise, T_pos position_depart);

T_joueur *create_list_joueur(T_banquise banquise, T_init_jeu init_jeu_data);

void remp_banquise_tab_edge(T_case **tab, int taille);

void debug_affichage(T_banquise banquise);

bool is_partie_finie(T_banquise banquise);

bool is_vec(char input);

void gestion_joueur(T_banquise banquise, int ID_joueur);

void piege_joueur(T_banquise banquise, int ID_joueur);

void place_rocher(T_banquise banquise, int ID_joueur);

int calculer_score(T_banquise banquise, int ID_joueur);

int *classer_joueur(T_banquise banquise);

/* Code Ines */
T_case **alloue(int n);

void remp_banquise_tab_aux(T_case **tab, int taille, int x, int y);

void remp_banquise_tab(T_case **tab, int n);

void choisir_case_arrive(T_case **tab, int taille);

void choisir_case_depart(T_case **tab, int taille);

bool isvalid(int x, int y, int r, int c);

void init_place_joueur(T_banquise *banquise);

void move_j_aux(T_banquise banquise, int joueur);

int pos_j_valide(T_banquise banquise, int joueur);

void mettre_case_j_null(T_banquise banquise, int joueur);

void deplacer_joueur(T_banquise banquise, int joueur);

void rechauffement_climatique(T_banquise banquise);

void move_glacon(T_banquise banquise, int joueur);

void Color(int couleurDuTexte, int couleurDeFond);

void init_glacon(T_case **tab, int taille);

void init_ressort(T_case **tab, int taille);

void init_piege(T_case **tab, int taille);

void init_rocher(T_case **tab, int taille);

void move_m_t(T_banquise banquise, T_vec *vec, T_pos new_pos, T_pos pos);

#endif // BANQUISE_H_INCLUDED
