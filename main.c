#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banquise.h"
#include "main.h"

#define WINVER 0x0500

#include <windows.h>

T_init_jeu init_jeu_data;

void menu_exit(T_banquise banquise) {
    print_banquise_game();
    int *classement_joueur = classer_joueur(banquise);
    for (int i = 0; i < banquise.nombre_joueur; i++){
       if (i == 0){
           fprintf(stdout, "Le gagnant est le joueur %s avec un score de %d ! Felicitations !\n", banquise.joueurs[classement_joueur[i]].nom, calculer_score(banquise, classement_joueur[i]));
       }
       else {
           fprintf(stdout, "Le joueur %s a termine %d eme avec un score de %d\n", banquise.joueurs[classement_joueur[i]].nom, i + 1, calculer_score(banquise, classement_joueur[i]));
       }
    }
    char c;
    fflush(stdin);
    scanf("%c", &c);
    if (c != 'd' && c != 's' && c != 'q' && c != 'z') {
        exit(1);
    }
}

void fullscreen() {
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1c, 0, 0);
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}

void menu_commandes() {
    print_banquise_game();
    printf("Les controles sont les suivants :\n"
           "- les touches Z, Q, S, D  servent �  deplacer les  personnage\n"
           " Z : se deplacer vers le haut \n"
           " S : se deplacer vers le bas \n"
           " Q : se deplacer vers la gauche\n"
           " D : se deplacer vers la droite \n"
           " f : permets de pouvoir placer un rocher sur la banquise puis vous devez mettre une position valide\n"
           "\n"
           "Sur la banquise on retrouve plusieurs symboles : \n"
           "A = Arrivee\n"
           "D = Depart\n"
           "\n"
           "* = Banquise ou bien la glace c'est les cases sur lesquels on se depace\n"
           "  =  Le vide repr�sente l'eau\n"
           "1/2/3/4 = Les chiffres de un � quatre repr�sente les Joueurs dans l'ordre \n"
           "\n"
           "M = Manche du marteau \n"
           "T = tete du marteau \n"
           "G = Glacon \n"
           "X = Ressort\n"
           "R = Rocher \n"
           "Attention il y a des Pieges invisibles un peu partout sur la banquise "); //TODO Completer
    char c = ' ';
    fflush(stdin);
    scanf("%c", &c);
    if (c != ' ') {
        init_jeu_data.statusMenu = 1;
    } else {
        init_jeu_data.statusMenu = 3;
    }
}

void menu_regles() {
    print_banquise_game();
    printf("Des regles vraiment interessantes et claires\n"
           "Les joueurs initialises sur la banquise vont se deplacer tour atour \n"
           "A chaque tour un joueurs fait un mouvement on utilisant les touches defini dans la section commandes \n"
           "Une partie prends fin des qu'un joueur arrivent a la case d'arrivee \n"
           "Si un joueur se deplace sur une case piege il sera bloque pendant un tour\n"
           "Un joueur peut faire bouger un glacon si il le souhaite \n"
           "Un glacon en mouvement peut agrandir la banquise ou bien tuer un joueurs\n"
           "Un ressot inverse le mouvement d'un glacon"
           "Un marteau peut changer la trajectoire du glacon en mouvement de 90 degres"); //TODO Completer
    char c = ' ';
    fflush(stdin);
    scanf("%c", &c);
    if (c != ' ') {
        init_jeu_data.statusMenu = 1;
    } else {
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
    char **charArray;
    charArray = malloc(init_jeu_data.nbJoueurs * sizeof(char *));
    for (int i = 0; i < init_jeu_data.nbJoueurs; i++) {
        charArray[i] = (char *) malloc(50 * sizeof(char));
    }
    for (int i = 0; i < init_jeu_data.nbJoueurs; i++) {
        print_banquise_game();
        char nom[50];
        printf("Entrez le nom du joueur %d :\n", i + 1);
        fflush(stdin);
        scanf("%s", nom);
        strcpy(charArray[i], nom);
    }
    init_jeu_data.nomJoueurs = charArray;
    print_banquise_game();
    printf("Choix de la taille de la banquise\n"
           "\n"
           "Compris entre 10 et 40\n");
    int taille;
    fflush(stdin);
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
            if (init_jeu_data.param == 0) {
                init_jeu_data.statusMenu = 4;
            } else {
                init_jeu_data.statusMenu = 0;
            }
            break;
        default:
            init_jeu_data.statusMenu = 1;
            break;
    }
    init_jeu_menu_manager();
}

void init_jeu_menu_manager() {
    switch (init_jeu_data.statusMenu) {
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

T_banquise init_jeu() {
    init_jeu_aux();
    T_banquise banquise = create_banquise(init_jeu_data.mapTaille, init_jeu_data.nbJoueurs);
    T_joueur *joueur = create_list_joueur(banquise, init_jeu_data);
    banquise.joueurs = joueur;
    affiche_banquise(banquise);
    return banquise;
}

int main() {
    /* Code Louis */
    fullscreen();
    T_banquise banquise = init_jeu();
    while (chemin_exist(banquise, position_depart(banquise)) == 0) {
        banquise.tab = create_tab(init_jeu_data.mapTaille);
    }
    bool statusPartie = true;
    // Boucle principale de jeu
    while (statusPartie) {
        for (int i = 0; i < banquise.nombre_joueur; i++) {
            if(banquise.joueurs[i].estEnVie == 1) {
                affiche_banquise(banquise);
                gestion_joueur(banquise, i);
                statusPartie = is_partie_finie(banquise);
                if (statusPartie == false) {
                    break;
                }
            }
        }
        rechauffement_climatique(banquise);
        banquise.tours += 1;
    }
    menu_exit(banquise);
    free(banquise.tab);
    free(banquise.joueurs);
    return 0;
}
