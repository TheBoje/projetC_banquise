#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include "banquise.h"

void print_banquise_game(){
    system("@cls||clear");
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

T_init_jeu init_jeu_joueurs(T_init_jeu init_jeu_data){
    print_banquise_game();
    printf("Nombre de joueurs : 1 a 4\n"
           "\n"
           "Entrez le nombre de joueurs\n");
    char c;
    scanf("%c", &c);
    int temp = (int)c - 48;
    if (temp >= 1 && temp <= 4){
        system("@cls||clear");
        init_jeu_data.nbJoueurs = temp;
        return init_jeu_data;
    }
    else {
        fflush(stdin);
        return init_jeu_joueurs(init_jeu_data);
    }
}

void print_init_jeu(T_init_jeu init_jeu){
    printf("Nb joueurs : %d\n", init_jeu.nbJoueurs);
    for (int i = 0; i < init_jeu.nbJoueurs; i++){
        printf("Joueur n%d, nom : %s\n", i + 1, init_jeu.nomJoueurs[i]);
    }
    printf("Taille de la carte : %d\n", init_jeu.mapTaille);
}

void menu_commandes(){
    print_banquise_game();
    printf("Les controles sont les suivants :\n"
           "- Z Q S D pour se déplacer avec le personnage\n"
           "- A COMPLETER\n"); //TODO Completer
           // TODO return to last menu
}

void menu_regles(){
    print_banquise_game();
    printf("A écrire\n"); //TODO Completer
    // TODO return to last menu
}

T_init_jeu menu_parametre(T_init_jeu init_jeu_data){
    print_banquise_game();
    printf("Nombre de joueurs : 1 a 4\n"
           "\n"
           "Entrez le nombre de joueurs\n");
    int temp;
    fflush(stdin);
    scanf("%d", &temp);
    if (temp < 1) {
        temp = 1;
    } else if (temp > 4) {
        temp = 4;
    }
    init_jeu_data.nbJoueurs = temp;
    for (int i = 0; i < init_jeu_data.nbJoueurs; i++) {
        print_banquise_game();
        //init_jeu_data.couleursJoueurs[i] = i;
        char nom[50];
        printf("Entrez le nom du joueur %d :\n", i + 1);
        scanf("%s", nom);
        init_jeu_data.nomJoueurs[i] = nom;
    }
    print_banquise_game();
    printf("Choix de la taille de la banquise\n"
           "\n"
           "Compris entre 10 et 40\n");
    int taille;
    scanf("%d", &taille);
    if (taille > 40){
        taille =  40;
    } else if (taille < 10) {
        taille = 10;
    }
    init_jeu_data.mapTaille = taille;
    return init_jeu_data;
}

T_init_jeu init_jeu_select_menu(T_init_jeu init_jeu_data){
    print_banquise_game();
    printf("Liste des menus, appuyez sur la touche correspondante :\n"
           "- \"r\" pour les règles\n"
           "- \"c\" pour les commandes\n"
           "- \"p\" pour ouvrir les parametres\n"
           "- \"q\" pour fermer le jeu\n"); //TODO force quit function
    char c;
    c = getchar();
    switch (c){
        case 'r': menu_regles();
            break;
        case 'c': menu_commandes();
            break;
        case 'p': init_jeu_data = menu_parametre(init_jeu_data);
            break;
        default: init_jeu_select_menu(init_jeu_data);
            break;
    }
    return init_jeu_data;
}

T_init_jeu init_jeu_aux(){
    T_init_jeu init_jeu_data;
    print_banquise_game();
    printf("Cree par Louis Leenart et Ines Mesmi\n"
           "\n"
           "Appuyez sur \"enter\" pour continuer\n");
    int c;
    c = getchar();
    while (c != 10){
        fflush(stdin);
        c = getchar();
    }
    init_jeu_data = init_jeu_select_menu(init_jeu_data);
    return init_jeu_data;
}


T_banquise init_jeu() {
    T_init_jeu init_jeu_data = init_jeu_aux();
    print_init_jeu(init_jeu_data);
    int c = 2;
    T_banquise banquise = create_banquise(25, c); // TODO Faire ça plus proprement
    T_joueur *joueur = create_list_joueur(banquise, c);
    //affiche_banquise(banquise);
    return banquise;
}


int main() {
    /* Code Louis */
    T_banquise banquise = init_jeu();
    printf("Chemin : %d\n", chemin_exist(banquise, position_depart(banquise)));
    free(banquise.tab);
    /* Code Ines */

    return 0;
}
