#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"


/* Code Louis */
void init_random() {
    int seed = time(NULL);
    srand(seed);
}

T_case **create_tab(int taille) {
    T_case **tab = alloue(taille);
    remp_banquise_tab(tab, taille);
    choisir_case_depart(tab, taille);
    choisir_case_arrive(tab, taille);
    return tab;
}

T_banquise create_banquise(int taille, int joueurs) {
    init_random();
    T_banquise banquise;
    banquise.tab = create_tab(taille);
    banquise.taille = taille;
    banquise.nombre_joueur = joueurs;
    return banquise;
}

void modif_type_case(T_banquise banquise, int x, int y, T_type_case type_case) {
    banquise.tab[x][y].type_case = type_case;
}

void modif_pos_joueur(T_banquise banquise, T_joueur joueur) {
    banquise.tab[joueur.position.posx][joueur.position.posy].joueur = NULL;
    banquise.tab[joueur.position.posx + joueur.vecteur.dx][joueur.position.posy + joueur.vecteur.dy].joueur = &joueur;
}

T_pos position_arrive (T_banquise banquise){
    T_pos result;
    result.posx = 0;
    result.posy = 0;
    for (int i = 0; i < banquise.taille ; i++) {
        for (int j = 0; j < banquise.taille ; i++) {
            if ( banquise.tab[i][j].but == arrive) {
                result.posx = i;
                result.posy = j;
                break;
            }
        }
    }
    return result;
}

int **create_tab_chemin(int taille) {
    int **tab = (int **) malloc(taille * sizeof(int *));
    for (int i = 0; i < taille; i++) {
        tab[i] = (int *) malloc(taille * sizeof(int));
    }
    for (int i = 0; i < taille; i++){
        for (int j = 0; j < taille; j++) {
            tab[i][j] = 0;
        }
    }
}

int **init_chemin_existe(T_banquise banquise, T_pos pos, int taille) {
    int **search = create_tab_chemin(banquise.taille);
}
int chemin_existe(T_banquise banquise, T_pos pos, int **search) {
    T_pos arrive = position_arrive(banquise);
    if (search[arrive.posx][arrive.posy] == 1){ // Dans tab, inconnu = 0, atteignable = 1, limite = 2.
        return 1;
    }
    else {
        for (int i = 0; i <= 3; i++){
            int offx = 0, offy = 0;
            switch (i){
                case 0:
                    offx = 1;
                    offy = 0;
                    break;
                case 1:
                    offx = 0;
                    offy = 1;
                    break;
                case 2:
                    offx = -1;
                    offy = 0;
                    break;
                case 3:
                    offx = 0;
                    offy = -1;
                    break;
                default:
                    break;
            }
            T_pos temp_pos = pos;
            temp_pos.posx = temp_pos.posx + offx;
            temp_pos.posy = temp_pos.posx + offy;
            if (search[temp_pos.posx][temp_pos.posy] == 0) {
                if (banquise.tab[temp_pos.posx][temp_pos.posx].type_case == glace
                 && banquise.tab[temp_pos.posx][temp_pos.posx].objet == vide)
                {
                    search[temp_pos.posx][temp_pos.posx] = 1;
                    chemin_existe(banquise, temp_pos, search);
                }
                else {
                    search[temp_pos.posx][temp_pos.posx] = 2;
                }
            }
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
            result = '1';
            break;
        default:
            result = ' ';
            break;
    }
    return result;
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
                tab[i][j].type_case = eau;
            }
        }
    }
}

void remp_banquise_tab(T_case **tab, int taille) {
    int i, j, x, y, r;
    r = rand() % (2 * taille) + taille;
    for (int k = 0; k < r; k++) {
        x = rand() % taille;
        y = rand() % taille;
        remp_banquise_tab_aux(tab, taille, x, y);
    }
    for (i = 0; i < taille; i++) {
        for (j = 0; j < taille; j++) {
            tab[i][j].but = defaut;
            tab[i][j].objet = vide;
        }
    }
}

void choisir_case_depart(T_case **tab, int taille) {
    int i, j;
    i = rand() % taille;
    j = rand() % taille;
    if (tab[i][j].type_case == glace) {
        choisir_case_depart(tab, taille);
    } else {
        tab[i][j].but = depart;

    }
}

void choisir_case_arrive(T_case **tab, int taille) {
    int i, j;
    i = rand() % taille;
    j = rand() % taille;
    if (tab[i][j].type_case == glace || tab[i][j].but == depart) {
        choisir_case_arrive(tab, taille);
    } else {
        tab[i][j].but = arrive;
    }
}


