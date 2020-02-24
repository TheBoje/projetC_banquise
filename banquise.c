#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>

#include "banquise.h"

/* Code Louis */
void init_random() {
    int seed = time(NULL);
    srand(seed);
}

T_case **create_tab(int taille, int joueurs) {
    T_case **tab = alloue(taille);
    remp_banquise_tab(tab, taille);
    choisir_case_depart(tab, taille);
    choisir_case_arrive(tab, taille);
    //init_place_joueur(tab,taille,&filedej);
    return tab;
}

T_banquise create_banquise(int taille, int joueurs) {
    init_random();
    T_banquise banquise;
    banquise.tab = create_tab(taille, joueurs);
    banquise.taille = taille;
    banquise.nombre_joueur = joueurs;
    return banquise;
}

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

T_pos offset_pos(T_pos pos, int offx, int offy) {
    pos.posx += offx;
    pos.posy += offy;
    return pos;
}

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

int **tab_chemin_fill_eau(T_banquise banquise, int **search) {
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            if (banquise.tab[i][j].type_case == eau) {
                search[i][j] = 2;
            }
        }
    }
    return search;
}

int chemin_exist(T_banquise banquise, T_pos pos) {
    int **search = tab_chemin_fill_eau(banquise, create_tab_chemin(banquise.taille));
    T_pos pos_arrive = position_arrive(banquise);
    return chemin_exist_aux(banquise, pos, pos_arrive, search);
}

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
                //print_search(search, banquise);
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
        } else if (search[pos.posx][pos.posy] == 2) {
            return 0;
        } else {
            return 0;
        }
    }
}

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

T_joueur *create_list_joueur(int nbJoueurs, T_pos position_depart) {
    T_joueur *joueur;
    joueur = (T_joueur *) malloc(nbJoueurs * sizeof(T_joueur));
    for (int i = 0; i < nbJoueurs; i++) {
        strcpy(joueur[i].nom, "Joueur");
        joueur[i].couleur = i;
        joueur[i].id = i;
        joueur[i].position.posx = position_depart.posx + i;
        joueur[i].position.posy = position_depart.posy;
        joueur[i].vecteur.dx = 0;
        joueur[i].vecteur.dy = 0;
        joueur[i].score.distance = 0;
        joueur[i].score.nb_glacon = 0;
        joueur[i].score.nb_victime = 0;
    }
    return joueur;
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
    int i, j, x, y, r;
    r = (rand() % (taille)) + taille;
    for (int k = 0; k < r; k++) {
        x = rand() % taille;
        y = rand() % taille;
        remp_banquise_tab_aux(tab, taille, x, y);
    }
    for (i = 0; i < taille; i++) {
        for (j = 0; j < taille; j++) {
            tab[i][j].but = defaut;
            tab[i][j].objet = vide;
            tab[i][j].joueur = NULL;
        }
    }
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

bool isvalid(int x, int y, int r, int c) {
    return (x >= 0 && y >= 0 && x < r && y < c);
}

void init_place_joueur(T_case **tab, int taille, file_j *f) {
    int i, j, x = r, y = c;
    cellule *sauv = f->tete;
    int count = 1;
    int step = 1;
    while ((r >= 0) && (c >= 0) && sauv != NULL) {
        for (j = y + 1; j <= y + step; j++)
            if ((isvalid(x, j, r, c)) && (x < taille) && (j < taille) && (tab[x][j].joueur == NULL) &&
                (tab[x][j].but != depart) && (tab[x][j].but != arrive) &&
                (tab[x][j].type_case == glace && (sauv != NULL))) {
                tab[x][j].joueur = &(sauv->joueur);
                tab[x][j].joueur->position.posx = x;
                tab[x][j].joueur->position.posy = y;
                sauv = sauv->suiv;
                count++;

            }

        y = y + step;
        for (i = x + 1; i <= x + step; i++)
            if ((isvalid(i, y, r, c) && (i < taille) && (y < taille) && tab[i][y].joueur == NULL) &&
                (tab[i][y].but != depart) && (tab[i][y].but != arrive) &&
                (tab[i][y].type_case == glace && (sauv != NULL))) {
                tab[i][y].joueur = &(sauv->joueur);
                tab[i][y].joueur->position.posx = i;
                tab[i][y].joueur->position.posy = y;
                count++;
                sauv = sauv->suiv;
            }
        x = x + step;
        step++;
        for (j = y - 1; j >= y - step; j--)
            if (isvalid(x, j, r, c) && (x < taille) && (j < taille) && (tab[x][j].joueur == NULL) &&
                (tab[x][j].but != depart) && (tab[x][j].but != arrive) &&
                (tab[x][j].type_case == glace && (sauv != NULL))) {
                tab[x][j].joueur = &(sauv->joueur);
                tab[x][j].joueur->position.posx = x;
                tab[x][j].joueur->position.posy = y;
                count++;
                sauv = sauv->suiv;
            }


        y = y - step;
        for (i = x - 1; i >= x - step; i--)
            if (isvalid(i, y, r, c) && (i < taille) && (y < taille) && (tab[i][y].joueur == NULL) &&
                (tab[i][y].but != depart) && (tab[i][y].but != arrive) &&
                (tab[i][y].type_case == glace && (sauv != NULL))) {
                tab[i][y].joueur = &(sauv->joueur);
                tab[i][y].joueur->position.posx = i;
                tab[i][y].joueur->position.posy = j;
                count++;
                sauv = sauv->suiv;
            }


        x = x - step;
        step++;
    }
}

void move_j_aux(T_case **tab, int taille, file_j *f) {

    cellule *c = f->tete;
    int i = c->joueur.position.posx;
    int j = c->joueur.position.posy;
    char m = getch();
    if (m == 'q' && j - 1 < taille && (tab[i][j - 1].joueur == NULL)) {
        tab[i][j].joueur = NULL;
        c->joueur.position.posy = j - 1;
        tab[i][j - 1].joueur = c;
        tab[i][j].joueur = NULL;

    }
    if (m == 'z' && i - 1 < taille && (tab[i - 1][j].joueur == NULL)) {
        tab[i][j].joueur = NULL;
        c->joueur.position.posx = i - 1;
        tab[i - 1][j].joueur = c;

    }
    if (m == 'd' && j + 1 < taille && (tab[i][j + 1].joueur == NULL)) {
        tab[i][j].joueur = NULL;
        c->joueur.position.posy = j - 1;
        tab[i][j + 1].joueur = c;

    }
    if (m == 's' && i + 1 < taille && (tab[i + 1][j].joueur == NULL)) {
        tab[i][j].joueur = NULL;
        c->joueur.position.posx = i - 1;
        tab[i + 1][j].joueur = c;
    }
}

void Color(int couleurDuTexte, int couleurDeFond) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}