#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banquise.h"
#include "main.h"
#define WINVER 0x0500
#include <windows.h>

T_init_jeu init_jeu_data;

void print_banquise_game() {
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
                fprintf(stdout, " %d ", banquise.tab[i][j].joueur->id + 1);
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

void menu_commandes() {
    print_banquise_game();
    printf("Les controles sont les suivants :\n"
           "- Z Q S D pour se deplacer avec le personnage\n"
           "A = Arrivee\n"
           "D = Depart\n"
           "* = Banquise\n"
           "  = Eau\n"
           "1/2/3/4 = Joueur\n"); //TODO Completer
    char c = ' ';
    scanf("%c", &c);
    if (c != ' '){
        fflush(stdin);
        init_jeu_data.statusMenu = 1;
    }
    else {
        init_jeu_data.statusMenu = 3;
    }
}

void menu_regles() {
    print_banquise_game();
    printf("Des regles vraiment interessantes et claires\n"); //TODO Completer
    char c = ' ';
    scanf("%c", &c);
    if (c != ' '){
        fflush(stdin);
        init_jeu_data.statusMenu = 1;
    }
    else {
        init_jeu_data.statusMenu = 2;
    }
}

void menu_parametre() {
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
    char** charArray;
    charArray =  malloc(init_jeu_data.nbJoueurs * sizeof(char*));
    for (int i = 0; i < init_jeu_data.nbJoueurs; i++) {
        charArray[i] = (char*)malloc(50*sizeof(char));
    }
    for (int i = 0; i < init_jeu_data.nbJoueurs; i++) {
        print_banquise_game();
        fflush(stdin);
        char nom[50];
        printf("Entrez le nom du joueur %d :\n", i + 1);
        scanf("%s", nom);
        strcpy(charArray[i], nom);
    }
    init_jeu_data.nomJoueurs = charArray;
    print_banquise_game();
    printf("Choix de la taille de la banquise\n"
           "\n"
           "Compris entre 10 et 40\n");
    int taille;
    scanf("%d", &taille);
    if (taille > 40) {
        taille = 40;
    } else if (taille < 10) {
        taille = 10;
    }
    init_jeu_data.mapTaille = taille;
    init_jeu_data.statusMenu = 1;
    init_jeu_data.param = 1;
}

void init_jeu_select_menu() {
    print_banquise_game();
    printf("Liste des menus, appuyez sur la touche correspondante :\n"
           "- \"r\" pour les regles\n"
           "- \"c\" pour les commandes\n"
           "- \"p\" pour ouvrir les parametres\n"
           "- \"q\" pour fermer le jeu\n"
           "- \"g\" pour lancer la partie\n");
    char c;
    fflush(stdin);
    scanf("%c", &c);
    switch (c) {
        case 'r':
            init_jeu_data.statusMenu = 2;
            break;
        case 'c':
            init_jeu_data.statusMenu = 3;
            break;
        case 'p':
            init_jeu_data.statusMenu = 4;
            break;
        case 'q':
            exit(1);
        case 'g':
            if (init_jeu_data.param == 0){
                init_jeu_data.statusMenu = 4;
            }
            else {
            init_jeu_data.statusMenu = 0;
            }
            break;
        default:
            init_jeu_data.statusMenu = 1;
            break;
    }
    init_jeu_menu_manager();
}

void init_jeu_menu_manager(){
    switch (init_jeu_data.statusMenu){
        case 0:
            break;
        case 1:
            init_jeu_select_menu();
            break;
        case 2:
            menu_regles();
            init_jeu_menu_manager();
            break;
        case 3:
            menu_commandes();
            init_jeu_menu_manager();
            break;
        case 4:
            menu_parametre();
            init_jeu_menu_manager();
            break;
        default:
            init_jeu_select_menu();
            break;
    }
}

void init_jeu_aux() {
    init_jeu_data.statusMenu = 1; // Status => 0 = joue, 1 = init menu, 2 = regles, 3 = commandes, 4 = parametres
    init_jeu_data.param = 0;
    print_banquise_game();
    printf("Cree par Louis Leenart et Ines Mesmi\n"
           "\n"
           "Appuyez sur \"enter\" pour continuer\n");
    int c;
    c = getchar();
    while (c != 10) {
        fflush(stdin);
        c = getchar();
    }
    init_jeu_select_menu();
}

T_vec char_to_t_vec(char c){
    T_vec result;
    switch (c){
        case 'z':
            result.dx = -1;
            result.dy = 0;
            break;
        case 'q':
            result.dx = 0;
            result.dy = -1;
            break;
        case 's':
            result.dx = 1;
            result.dy = 0;
            break;
        case 'd':
            result.dx = 0;
            result.dy = 1;
            break;
        default:
            result.dx = 0;
            result.dy = 0;
            break;
    }
    return result;
}

T_banquise init_jeu() {
    init_jeu_aux();
    T_banquise banquise = create_banquise(init_jeu_data.mapTaille, init_jeu_data.nbJoueurs);
    T_joueur *joueur = create_list_joueur(banquise, init_jeu_data);
    banquise.joueurs = joueur;
    return banquise;
}


int main() {
    /* Code Louis */
    T_banquise banquise = init_jeu();
    while (chemin_exist(banquise, position_depart(banquise)) == 0){
        banquise.tab = create_tab(init_jeu_data.mapTaille);
    }
    HWND wh = GetConsoleWindow(); // Récupération de la console windows dans laquelle le jeu est affiché
    MoveWindow(wh, 0, 0, 1000, 1000, TRUE); // Agrandissement de la taille de la console
    affiche_banquise(banquise);
    //printf("Chemin : %d\n", chemin_exist(banquise, position_depart(banquise)));
    int nbTours = 0;
    bool statusPartie = true;
    // Boucle principale de jeu
    while (banquise.nombre_joueur > 0 && statusPartie)
    {
        for (int i = 0; i < banquise.nombre_joueur; i++){
            affiche_banquise(banquise);
            char input;
            T_vec vec;
            debug_affichage(banquise);
            printf("Joueur %d input :\n", i+1);
            fflush(stdin);
            scanf("%c", &input);
            vec = char_to_t_vec(input);
            banquise.joueurs[i].vecteur = vec;
            move_tour(banquise, i);
            statusPartie = is_partie_finie(banquise);
            if (statusPartie == false){
                break;
            }
        }
        nbTours += 1;
    }
    print_banquise_game();
    T_joueur gagnant;
    T_pos arrive = position_arrive(banquise);
    gagnant = *banquise.tab[arrive.posx][arrive.posy].joueur;
    fprintf(stdout, "Le gagnant est le joueur %s ! Felicitations !\n", gagnant.nom);
    char c;
    fflush(stdin);
    scanf("%c", &c);
    if (c != 'd' && c != 's' && c != 'q' && c != 'z'){
        exit(1);
    }
    free(banquise.tab);

    return 0;
}
