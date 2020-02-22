#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <w32api/minmax.h>
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
        for (int j = 0; j < banquise.taille ; j++) {
            if (banquise.tab[i][j].but == arrive) {
                result.posx = i;
                result.posy = j;
            }
        }
    }
    return result;
}

T_pos position_depart(T_banquise banquise){
    T_pos result;
    result.posx = 0;
    result.posy = 0;
    for (int i = 0; i < banquise.taille ; i++) {
        for (int j = 0; j < banquise.taille ; j++) {
            if (banquise.tab[i][j].but == depart) {
                result.posx = i;
                result.posy = j;
            }
        }
    }
    return result;
}

T_pos offset_pos(T_pos pos, int offx, int offy){
    pos.posx += offx;
    pos.posy += offy;
    return pos;
}

int is_in_banquise(T_banquise banquise, T_pos pos){
    if ((pos.posx < 0)
        || (pos.posy < 0)
        || (pos.posx >= banquise.taille)
        || (pos.posy >= banquise.taille)
        )
    {
        return 0;
    }
    else
    {
        return 1;
    }
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
    return tab;
}

int chemin_existe(T_banquise banquise, T_pos pos) {
    int **search = create_tab_chemin(banquise.taille);
    T_pos pos_arrive = position_arrive(banquise);
    return chemin_exist_aux(banquise, pos, pos_arrive, search);
}

int chemin_exist_aux(T_banquise banquise, T_pos pos, T_pos pos_arrive, int **search){
    if (is_in_banquise(banquise, pos) == 0 || search[pos.posx][pos.posy] == 1)
    {
        return 0;
    }
    else if (banquise.tab[pos.posx][pos.posy].but == arrive){
        search[pos.posx][pos.posy] = 1;
        return 1;
    }
    else {
        int r1 = 0, r2 = 0, r3 = 0, r4 = 0;
        /* Todo vérifier que pos + offset appartient à banquise.taille sinon crash
            Fonction int result_search(search, pos, offset) qui regarde pos + offset ?*/
        if (search[pos.posx + 1][pos.posy] == 0){
            r1 = chemin_exist_aux(banquise, offset_pos(pos,  1, 0), pos_arrive, search);
        }
        if (search[pos.posx - 1][pos.posy] == 0) {
            r2 = chemin_exist_aux(banquise, offset_pos(pos, -1, 0), pos_arrive, search);
        }
        if (search[pos.posx][pos.posy + 1] == 0) {
            r3 = chemin_exist_aux(banquise, offset_pos(pos, 0,  1), pos_arrive, search);
        }
        if (search[pos.posx][pos.posy - 1] == 0) {
            r4 = chemin_exist_aux(banquise, offset_pos(pos, 0, -1), pos_arrive, search);
        }

        if (r1 || r2 || r3 || r4)
        {
            return 1;
        }
        else
        {
            search[pos.posx][pos.posy] = 1;
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


