#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "banquise.h"


void affiche_banquise(T_banquise banquise) {
    int i, j;
    for (i = 0; i < banquise.taille; i++) {
        for (j = 0; j < banquise.taille; j++) {
            if (banquise.tab[i][j].joueur != NULL) {
                fprintf(stdout, " %c ", 'X');
            } else if (banquise.tab[i][j].but != defaut) {
                fprintf(stdout, " %c ", T_but_to_char(banquise.tab[i][j].but));
            } else if (banquise.tab[i][j].objet != vide) {
                fprintf(stdout, " %c ", T_objet_to_char(banquise.tab[i][j].objet));
            } else {
                fprintf(stdout, " %c ", T_case_to_char(banquise.tab[i][j].type_case));
            }
        }
        fprintf(stdout, "\n");
    }
}

int init_jeu_joueurs(){
    printf("Jeu de la banquise\n"
           "\n"
           "Nombre de joueurs : 1 à 4\n"
           "\n"
           "Entrez le nombre de joueurs\n");
    int c;
    c = getchar();
    printf("%d", c);
    if (c >= 1 && c <= 4){
        return c;
    }
    else {
        system("@cls||clear");
        return init_jeu_joueurs();
    }
}

int init_jeu_aux(){
    printf("Jeu de la Banquise\n"
           "\n"
           "Créé par Louis Leenart et Ines Mesmi\n"
           "\n"
           "Appuyez sur \"enter\" pour continuer\n"
    );
    int c;
    c = getchar();
    printf("%d", c);
    if(c == 10){
        system("@cls||clear");
        return init_jeu_joueurs();
    }
    else {
        system("@cls||clear");
        return init_jeu_aux();
    }
}


T_banquise init_jeu() {
    init_jeu_aux();
    T_banquise banquise = create_banquise(20, 1);
    affiche_banquise(banquise);
    return banquise;
}


int main() {
    /* Code Louis */
    T_banquise banquise = init_jeu();
    printf("Chemin : %d\n", chemin_exist(banquise, position_depart(banquise)));

    /* Code Ines */

    return 0;
}
