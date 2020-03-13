#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <windows.h>
#include <stdbool.h>

#include "banquise.h"

#define FONTE_GLACE 5

/* Code Louis */

/* Initialise le random pour la session
 * A lancer une fois par jeu
 * Genère une seed à partir de l'horloge internet de l'ordinateur
 * */
void init_random() {
    int seed = time(NULL);
    srand(seed);
}

/* Créé le tableau de jeu de T_banquise (tab)
 * Taille : taille du tableau (taille * taille)
 * Retourne le tableau de T_case representant la banquise
 * Inclus l'allocation mémoire du tableau, initialisation des cases, et le placement des cases de départ et d'arrivee
 * */
T_case **create_tab(int taille) {
    T_case **tab = alloue(taille);
    remp_banquise_tab(tab, taille);
    choisir_case_depart(tab, taille);
    choisir_case_arrive(tab, taille);
    return tab;
}

/* Créé la banquise
 * Taille : taille du tableau (taille * taille)
 * Retourne le T_banquise contenant les variables du jeu
 * Inclus l'initialisation du random et l'initialisation des différents variables
 * */
T_banquise create_banquise(int taille, int joueurs) {
    init_random();
    T_banquise banquise;
    banquise.taille = taille;
    banquise.nombre_joueur = joueurs;
    do{
        banquise.tab = create_tab(taille);
    }
    while (chemin_exist(banquise, position_depart(banquise)) == 0);
    return banquise;
}

/* Retourne la position d'arrive à partir de la banquise
 * Complexite au pire O(n²), n taille de la banquise
 */
T_pos position_arrive(T_banquise banquise) {
    T_pos result;
    result.posx = 0;
    result.posy = 0;
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            if (banquise.tab[i][j].but == arrive) {
                result.posx = i;
                result.posy = j;
            }
        }
    }
    return result;
}

/* Retourne la position de depart à partir de la banquise
 * Complexite au pire O(n²), n taille de la banquise
 * */
T_pos position_depart(T_banquise banquise) {
    T_pos result;
    result.posx = 0;
    result.posy = 0;
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            if (banquise.tab[i][j].but == depart) {
                result.posx = i;
                result.posy = j;
            }
        }
    }
    return result;
}

/* Permet d'offset une position selon x et y
 * Pos + x (selon x) et Pos + y (selon y)
 * */
T_pos offset_pos(T_pos pos, int offx, int offy) {
    pos.posx += offx;
    pos.posy += offy;
    return pos;
}

/* Verifie si la T_pos est dans banquise.tab
 * 0 = n'est pas dans la banquise (out of bounds)
 * 1 = est dans la banquise
 * */
int is_in_banquise(T_banquise banquise, T_pos pos) {
    if ((pos.posx < 0)
        || (pos.posy < 0)
        || (pos.posx >= banquise.taille)
        || (pos.posy >= banquise.taille)
            ) {
        return 0;
    } else {
        return 1;
    }
}

/* Affiche l'état du tableau search pour retracer l'état de chemin_exist
 * */
void print_search(int **search, T_banquise banquise) {
    printf("------------------ SEARCH COMMENCE ------------------\n");
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            printf(" %d ", search[i][j]);
        }
        printf("\n");
    }
    printf("------------------ SEARCH TERMINEE ------------------\n");
}

/* Créé le tableau nécessaire à la recherche de chemin dans chemin_exist
 * Taille = taille du tableau retourné. Doit être de la même taille que banquise.taille
 * Alloue l'espace mémoire, et initialise les valeurs à 0
 * */
int **create_tab_chemin(int taille) {
    int **tab = (int **) malloc(taille * sizeof(int *));
    for (int i = 0; i < taille; i++) {
        tab[i] = (int *) malloc(taille * sizeof(int));
    }
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            tab[i][j] = 0;
        }
    }
    return tab;
}

/* Fonction annexe de chemin_exist_aux
 * Associe la valeur 2 dans search à tous les points qui sont de l'eau dans banquise.taille
 * Permet de gagner de la lisibilité et de reduire le nombre d'opérations dans chemin_exist_aux
 * */
int **tab_chemin_fill_eau(T_banquise banquise, int **search) {
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            if (banquise.tab[i][j].type_case == 0) {
                search[i][j] = 2;
            }
        }
    }
    return search;
}

/* Determine l'existence d'un chemin entre pos et la case d'arrive
 * 0 = pas de chemin existant
 * 1 = au moins un chemin possible
 * Appelle la fonction récursive chemin_exist_aux()
 * */
int chemin_exist(T_banquise banquise, T_pos pos) {
    int **search = tab_chemin_fill_eau(banquise, create_tab_chemin(banquise.taille));
    //print_search(search, banquise);
    T_pos pos_arrive = position_arrive(banquise);
    int result = chemin_exist_aux(banquise, pos, pos_arrive, search);
    free(search);
    return result;
}

/* Fonction récursive cherchant un chemin entre pos et pos_arrive
 * Inscrit dans search les cases déjà analysés
 * Si la case est accessible, alors appelle la fonction pour les cases adjascentes
 * */
int chemin_exist_aux(T_banquise banquise, T_pos pos, T_pos pos_arrive, int **search) {
    if (search[pos.posx][pos.posy] == 0) {
        if (banquise.tab[pos.posx][pos.posy].type_case == eau) {
            search[pos.posx][pos.posy] = 2;
            return 0;
        } else {
            if (pos.posx == pos_arrive.posx && pos.posy == pos_arrive.posy) {
                search[pos.posx][pos.posy] = 3;
                return 1;
            } else {
                search[pos.posx][pos.posy] = 1;
                int r1 = 0, r2 = 0, r3 = 0, r4 = 0;
                if (is_in_banquise(banquise, offset_pos(pos, 1, 0))) {
                    r1 = chemin_exist_aux(banquise, offset_pos(pos, 1, 0), pos_arrive, search);
                }
                if (is_in_banquise(banquise, offset_pos(pos, -1, 0))) {
                    r2 = chemin_exist_aux(banquise, offset_pos(pos, -1, 0), pos_arrive, search);
                }
                if (is_in_banquise(banquise, offset_pos(pos, 0, 1))) {
                    r3 = chemin_exist_aux(banquise, offset_pos(pos, 0, 1), pos_arrive, search);
                }
                if (is_in_banquise(banquise, offset_pos(pos, 0, -1))) {
                    r4 = chemin_exist_aux(banquise, offset_pos(pos, 0, -1), pos_arrive, search);
                }
                if (r1 == 1 || r2 == 1 || r3 == 1 || r4 == 1) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    } else {
        if (search[pos.posx][pos.posy] == 3) {
            return 1;
        } else {
            return 0;
        }
    }
}

/* Converti T_but en charactere pour l'affichage
 * */
char T_but_to_char(T_but objet) {
    char result;
    switch (objet) {
        case 0: // Depart
            result = 'D';
            break;
        case 1: // Arrivee
            result = 'A';
            break;
        default:
            result = ' ';
            break;
    }
    return result;
}

/* Converti T_case en charactere pour l'affichage
 * */
char T_case_to_char(T_type_case c) {
    char result;
    switch (c) {
        case 0:
            result = ' ';
            break;
        case 1:
            result = '*';
            break;
        default:
            result = ' ';
            break;
    }
    return result;
}

/* Place le joueur dans une position aléatoire [-2;2] selon x et y de la case de depart
 * Fonction auxiliere de create_list_joueur()
 * */
T_pos joueur_position(T_banquise banquise, T_pos position_depart) {
    int l, m;
    T_pos temp;
    do {
        l = 2 - (rand() % 5);
        m = 2 - (rand() % 5);
        temp = offset_pos(position_depart, l, m);
    } while (is_in_banquise(banquise, temp) == 0 || banquise.tab[temp.posx][temp.posy].type_case != glace ||
             banquise.tab[temp.posx][temp.posy].but != defaut || banquise.tab[temp.posx][temp.posy].joueur != NULL);
    return temp;
}

/* Initialise la liste des joueurs
 * nbJoueurs = nombre de joueurs
 * */
T_joueur *create_list_joueur(T_banquise banquise, T_init_jeu init_jeu_data) {
    T_joueur *joueur;
    joueur = (T_joueur *) malloc(init_jeu_data.nbJoueurs * sizeof(T_joueur));
    T_pos depart = position_depart(banquise);
    for (int i = 0; i < init_jeu_data.nbJoueurs; i++) {
        strcpy(joueur[i].nom, init_jeu_data.nomJoueurs[i]);
        joueur[i].couleur = i;
        joueur[i].id = i;
        joueur[i].vecteur.dx = 0;
        joueur[i].vecteur.dy = 0;
        joueur[i].score.distance = 0;
        joueur[i].score.nb_glacon = 0;
        joueur[i].score.nb_victime = 0;
        T_pos pos_joueur = joueur_position(banquise, depart);
        joueur[i].position = pos_joueur;
        banquise.tab[pos_joueur.posx][pos_joueur.posy].joueur = &joueur[i];
    }

    return joueur;
}


void remp_banquise_tab_edge(T_case **tab, int taille) {
    int **search = create_tab_chemin(taille);
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            if (tab[i][j].type_case == 0) {
                int r1 = 0, r2 = 0, r3 = 0, r4 = 0;
                if (i + 1 < taille) {
                    if (tab[i + 1][j].type_case == 1) {
                        r1 = 1;
                    }
                }
                if (i - 1 >= 0) {
                    if (tab[i - 1][j].type_case == 1) {
                        r2 = 1;
                    }
                }
                if (j + 1 < taille) {
                    if (tab[i][j + 1].type_case == 1) {
                        r3 = 1;
                    }
                }
                if (j - 1 >= 0) {
                    if (tab[i][j - 1].type_case == 1) {
                        r4 = 1;
                    }
                }
                if (r1 || r2 || r3 || r4) {
                    search[i][j] = 1;
                }
            }
        }
    }
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            if (search[i][j] == 1 && rand() % 2 == 0) {
                tab[i][j].type_case = 1;
            }
        }
    }
}

void debug_affichage(T_banquise banquise) {
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        fprintf(stdout, "joueur %d\t\t", i + 1);
    }
    fprintf(stdout, "\n");
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        fprintf(stdout, "pos : %d %d\t\t", banquise.joueurs[i].position.posx, banquise.joueurs[i].position.posy);
    }
    fprintf(stdout, "\n");
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        fprintf(stdout, "vec : %d %d\t\t", banquise.joueurs[i].vecteur.dx, banquise.joueurs[i].vecteur.dy);
    }
    fprintf(stdout, "\n");
}

bool is_partie_finie(T_banquise banquise) {
    T_pos arrive = position_arrive(banquise);
    if (banquise.tab[arrive.posx][arrive.posy].joueur == NULL) {
        return true;
    } else {
        return false;
    }
}

/* Code Ines */

T_case **alloue(int n) {
    int h;
    T_case **tab = (T_case **) malloc(n * sizeof(T_case *));
    for (h = 0; h < n; h++) {
        tab[h] = (T_case *) malloc(n * sizeof(T_case));
    }
    return tab;
}


void remp_banquise_tab_aux(T_case **tab, int taille, int x, int y) {
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i < taille && i >= 0 && j < taille && j >= 0) {
                tab[i][j].type_case = glace; // La glace est représenté par * dans la console
            }
        }
    }
}

void remp_banquise_tab(T_case **tab, int taille) {
    int x, y, r;
    r = 2 * taille;
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            tab[i][j].but = defaut;
            tab[i][j].objet = vide;
            tab[i][j].joueur = NULL;
            tab[i][j].type_case = eau;
        }
    }
    for (int k = 0; k < r; k++) {
        x = rand() % taille;
        y = rand() % taille;
        remp_banquise_tab_aux(tab, taille, x, y);
    }
    remp_banquise_tab_edge(tab, taille);
}

void choisir_case_depart(T_case **tab, int taille) {
    int i, j;
    i = rand() % taille;
    j = rand() % taille;
    while (tab[i][j].type_case == eau) {
        i = rand() % taille;
        j = rand() % taille;
    }
    tab[i][j].but = depart;
}

void choisir_case_arrive(T_case **tab, int taille) {
    int i, j;
    i = rand() % taille;
    j = rand() % taille;
    while (tab[i][j].type_case == eau) {
        i = rand() % taille;
        j = rand() % taille;
    }
    tab[i][j].but = arrive;
}

void move_j_aux(T_banquise banquise, int joueur) {
    T_pos pos = banquise.joueurs[joueur].position;
    T_vec vec = banquise.joueurs[joueur].vecteur;
    T_pos new_pos;
    new_pos.posx = pos.posx + vec.dx;
    new_pos.posy = pos.posy + vec.dy;
    banquise.tab[new_pos.posx][new_pos.posy].joueur = &banquise.joueurs[joueur];
}

// dire si la nouvelle case du joueur est valide
int pos_j_valide(T_banquise *banquise, int p) {
    int i = banquise->joueurs[p].position.posx + banquise->joueurs[p].vecteur.dx;
    int j = banquise->joueurs[p].position.posy + banquise->joueurs[p].vecteur.dy;
    if (i < 0 || i >= banquise->taille || j < 0 || j >= banquise->taille) {
        return 0;
    } else if (banquise->tab[i][j].type_case == eau || banquise->tab[i][j].joueur != NULL) {
        return 0;
    } else {
        return 1;
    }
}

// quand un joueur n'est plus sur une case valide il ne doit plus etre sur la banquise.
void mettre_case_j_null(T_banquise banquise, int joueur) {
    T_pos pos = banquise.joueurs[joueur].position;
    banquise.tab[pos.posx][pos.posy].joueur = NULL;
}

// faire passer les joueurs un par un
void move_tour(T_banquise banquise, int joueur) {
    T_pos pos_joueur = banquise.joueurs[joueur].position;
    T_vec vec_joueur = banquise.joueurs[joueur].vecteur;
    if (pos_j_valide(&banquise, joueur) == 1) {
        T_pos new_pos;
        new_pos.posx = pos_joueur.posx + vec_joueur.dx;
        new_pos.posy = pos_joueur.posy + vec_joueur.dy;
        T_vec vec_null;
        vec_null.dx = 0;
        vec_null.dy = 0;
        move_j_aux(banquise, joueur);
        mettre_case_j_null(banquise, joueur);
        banquise.joueurs[joueur].position = new_pos;
        banquise.joueurs[joueur].vecteur = vec_null;
    }
}

void rechauffement_climatique(T_banquise banquise) {
    int **search = create_tab_chemin(banquise.taille);
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            if (banquise.tab[i][j].type_case == 1) {
                int r1 = 0, r2 = 0, r3 = 0, r4 = 0;
                if (i + 1 < banquise.taille) {
                    if (banquise.tab[i + 1][j].type_case == 0) {
                        r1 = 1;
                    }
                }
                if (i - 1 >= 0) {
                    if (banquise.tab[i - 1][j].type_case == 0) {
                        r2 = 1;
                    }
                }
                if (j + 1 < banquise.taille) {
                    if (banquise.tab[i][j + 1].type_case == 0) {
                        r3 = 1;
                    }
                }
                if (j - 1 >= 0) {
                    if (banquise.tab[i][j - 1].type_case == 0) {
                        r4 = 1;
                    }
                }
                if (r1 || r2 || r3 || r4) {
                    search[i][j] = 1;
                }
            }
        }
    }
    for (int i = 0; i < banquise.taille; ++i) {
        for (int j = 0; j < banquise.taille; ++j) {
            if(search[i][j] == 1 && banquise.tab[i][j].joueur == NULL && banquise.tab[i][j].but == 2 && banquise.tab[i][j].objet == 6 && rand()%100 < FONTE_GLACE){
                banquise.tab[i][j].type_case = 0;
            }
        }
    }
}


void Color(int couleurDuTexte, int couleurDeFond) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}
