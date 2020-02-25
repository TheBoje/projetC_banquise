#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "banquise.h"

void print_banquise_game(){
    printf(" ____                          _             ____                      \n"
           "| __ )  __ _ _ __   __ _ _   _(_)___  ___   / ___| __ _ _ __ ___   ___ \n"
           "|  _ \\ / _` | '_ \\ / _` | | | | / __|/ _ \\ | |  _ / _` | '_ ` _ \\ / _ \\\n"
           "| |_) | (_| | | | | (_| | |_| | \\__ \\  __/ | |_| | (_| | | | | | |  __/\n"
           "|____/ \\__,_|_| |_|\\__, |\\__,_|_|___/\\___|  \\____|\\__,_|_| |_| |_|\\___|\n"
           "                      |_|                                              \n"
           "\n");
}

void affiche_banquise(T_banquise banquise) {
    print_banquise_game();
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
    print_banquise_game();
    printf("Nombre de joueurs : 1 a 4\n"
           "\n"
           "Entrez le nombre de joueurs\n");
    char c;
    scanf("%c", &c);
    int temp = (int)c - 48;
    if (temp >= 1 && temp <= 4){
        system("@cls||clear");
        return temp;
    }
    else {
        fflush(stdin);
        return init_jeu_joueurs();
    }
}

int init_jeu_aux(){
    printf("Cree par Louis Leenart et Ines Mesmi\n"
           "\n"
           "Appuyez sur \"enter\" pour continuer\n"
    );
    int c;
    c = getchar();
    while (c != 10){
        fflush(stdin);
        c = getchar();
    }
    system("@cls||clear");
    return init_jeu_joueurs();
}


T_banquise init_jeu() {
    int c = init_jeu_aux();
    T_banquise banquise = create_banquise(25, c); // TODO Faire ça plus proprement
    T_joueur *joueur = create_list_joueur(banquise, c);
    affiche_banquise(banquise);
    return banquise;
}


int main() {
    /* Code Louis */
    print_banquise_game();
    T_banquise banquise = init_jeu();
    printf("Chemin : %d\n", chemin_exist(banquise, position_depart(banquise)));
    free(banquise.tab);
    /* Code Ines */

    return 0;
}
