
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define WINVER 0x0500

#include <windows.h>
#include <stdbool.h>

#include "banquise.h"

#define FONTE_GLACE 1 // 0.1% de chances de fonte par tour pour chaque glace adjacent à de l'eau
#define NB_GLACON 15 // Nombre de glacon généré dans la banquise, voir init_glacon()

/* Initialise le random pour la session
 * A lancer une fois par jeu
 * Genère une seed à partir de l'horloge internet de l'ordinateur
 * */
void init_random() {
    int seed = time(NULL);
    srand(seed);
}
/*  transformer un char z,q,s,d on vecteur
 *   (0,1)(0,-1)(1,0) (-1,0) pour traduire les differentes directions possibles
 *  complexité : O(1)
*/
/*
 * Permet de conventir les input de char en T_vec (int dx, int dy)
 * */
T_vec char_to_t_vec(char c) {
    T_vec result;
    switch (c) {
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
/* Imprime le nom du jeu
*/
/* Affiche "BanquiseGame" en haut de l'écran
 * Appelé à chaque actualisation de l'affichage
 * */
void print_banquise_game() {
    system("@cls||clear");
    Color(3, 0);
    printf(" ____                          _             ____                      \n"
           "| __ )  __ _ _ __   __ _ _   _(_)___  ___   / ___| __ _ _ __ ___   ___ \n"
           "|  _ \\ / _` | '_ \\ / _` | | | | / __|/ _ \\ | |  _ / _` | '_ ` _ \\ / _ \\\n"
           "| |_) | (_| | | | | (_| | |_| | \\__ \\  __/ | |_| | (_| | | | | | |  __/\n"
           "|____/ \\__,_|_| |_|\\__, |\\__,_|_|___/\\___|  \\____|\\__,_|_| |_| |_|\\___|\n"
           "                      |_|                                              \n"
           "\n");
    Color(15, 0);
}
/* Affiche le jeu prends la banquise et l'affiche case par case selon le contenu
* Complexité : O(taille*taille) -> depends de la taille de la banquise
*/
/* Affiche le tableau de jeu de la banquise
 * Les éléments de la case sont affichés par ordre de priorité
 * Joueur > But > Objet > Type_case
 * Note: les pièges sont stockés dans tab[][].objet mais sont représentés par des '*'
 * */
void affiche_banquise(T_banquise banquise) {
    print_banquise_game();
    int i, j;
    for (i = 0; i < banquise.taille; i++) {         // Parcours de tout le tableau selon x
        for (j = 0; j < banquise.taille; j++) {     // et selon y

            if (banquise.tab[i][j].joueur != NULL) {
                Color(14, 0);
                fprintf(stdout, " %d ", banquise.tab[i][j].joueur->id + 1);
            } else if (banquise.tab[i][j].but != defaut) {
                Color(12, 0);
                fprintf(stdout, " %c ", T_but_to_char(banquise.tab[i][j].but));
            } else if (banquise.tab[i][j].objet != vide) {
                 if(banquise.tab[i][j].objet == piege){
                   Color(3,0);
                fprintf(stdout, " %c ", T_objet_to_char(banquise.tab[i][j].objet));
                 } else {
                Color(8,0);
                fprintf(stdout, " %c ", T_objet_to_char(banquise.tab[i][j].objet));
                 }
            } else {
                Color(3, 0);
                fprintf(stdout, " %c ", T_case_to_char(banquise.tab[i][j].type_case));
            }
        }
        fprintf(stdout, "\n");
    }
    Color(15, 0);
}

/* Créé le tableau de jeu de T_banquise (tab)
 * Taille : taille du tableau (taille * taille)
 * Retourne le tableau de T_cases representant la banquise
 * Inclus l'allocation mémoire du tableau, initialisation des cases, et le placement des cases de départ et d'arrivee, et de tous les objets
 * et le placement des joueurs ainsi que les objets: glaçon, ressort, marteau, piege, rocher et marteau
 * Complexité: cette fonction fait appel d'autre fonction donc sa complexité depends de la somme totale des autres
 * */
T_case **create_tab(int taille) {
    T_case **tab = alloue(taille);
    remp_banquise_tab(tab, taille);
    choisir_case_depart(tab, taille);
    choisir_case_arrive(tab, taille);
    init_glacon(tab, taille);
    init_ressort(tab, taille);
    init_piege(tab, taille);
    init_rocher(tab, taille);
    init_marteau(tab, taille);
    return tab;
}

/* Créé la banquise
 * Taille : taille du tableau (taille * taille)
 * Retourne le T_banquise contenant les variables du jeu
 * Inclus l'initialisation du random et l'initialisation des différents variables
 * La création du tableau de la banquise est répété tant qu'il n'existe pas au moins 1 chemin (voir chemin_exist())
 * */
T_banquise create_banquise(int taille, int joueurs) {
    init_random();
    T_banquise banquise;
    banquise.taille = taille;
    banquise.nombre_joueur = joueurs;
    banquise.tours = 0;
    do {
        banquise.tab = create_tab(taille);
    } while (chemin_exist(banquise, position_depart(banquise)) == 0);
    return banquise;
}

/* Retourne la position d'arrive à partir de la banquise
 * Complexite au pire O(n²), avec n taille de la banquise
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
 * Complexité:Constante
 */
T_pos offset_pos(T_pos pos, int offx, int offy) {
    pos.posx += offx;
    pos.posy += offy;
    return pos;
}

/* Verifie si la T_pos est dans banquise.tab
 * 0 = n'est pas dans la banquise (out of bounds)
 * 1 = est dans la banquise
 *Complexité: Constante
  */
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
 * Uniquement utilisé pour le debug dans chemin_exist()
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
 * Complexité : O(taille*taille)
 */
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
 * Complexité: O(taille*taille)
 */
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
 * Complexité: depend de la complexité de chemain_exist_aux
*/
int chemin_exist(T_banquise banquise, T_pos pos) {
    int **search = tab_chemin_fill_eau(banquise, create_tab_chemin(banquise.taille)); // initialise le tableau de recherche
    //print_search(search, banquise);
    T_pos pos_arrive = position_arrive(banquise);
    int result = chemin_exist_aux(banquise, pos, pos_arrive, search); // Fonction reccursive de recherche de chemin
    free(search); // libération du tableau de recherche
    return result;
}

/* Fonction récursive cherchant un chemin entre pos et pos_arrive
 * Analyse chaque case suivant l'idée de l'algorithme de l'A* (ou AStar)
 * Utilise un tableau d'int pour ne pas regarder deux fois sur la même case (et donc ne pas faire de boucle infinie)
 * A chaque itération, on cherche si les cases adjascentes sont accessibles, et si oui, si elles sont la case de destination
 * Si une des cases accessible est l'objectif, alors la fonction retourne 1, sinon on retourne 0
 * complexité au pire : O(n²), avec n la taille de la banquise
 * Complexité :
*/
int chemin_exist_aux(T_banquise banquise, T_pos pos, T_pos pos_arrive, int **search) {
    if (search[pos.posx][pos.posy] == 0) {      // Vérification que l'on n'a pas encore cherché dans cette case
        if (pos.posx == pos_arrive.posx && pos.posy == pos_arrive.posy) { // Cas ou la case est l'objectif
            search[pos.posx][pos.posy] = 3;
            return 1;
        } else {    // Reste des cas, et donc aussi des cases accessibles
            search[pos.posx][pos.posy] = 1; // On indique dans le tableau de recherche que la case est analysée
            int r1 = 0, r2 = 0, r3 = 0, r4 = 0; // On lance la fonction sur les cases adjascentes
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
            if (r1 == 1 || r2 == 1 || r3 == 1 || r4 == 1) { // Si l'une des cases retourne 1, donc un chemin, alors on retourne 1 aussi
                return 1;
            } else {
                return 0;
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

/* Permet de convertir un T_but en caractères, essentiel pour l'affichage
 * Complexité : Constante
*/
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

/* Permet de convertir une T_case en caractères, essentiel pour l'affichage
 * Complexité : Constante
 */
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
 * Fonction auxilière de create_list_joueur()
 * Compléxité: depend du rand()
*/
T_pos joueur_position(T_banquise banquise, T_pos position_depart) {
    int l, m;
    T_pos temp;
    do {
        l = 2 - (rand() % 5);
        m = 2 - (rand() % 5);
        temp = offset_pos(position_depart, l, m);
    } while (is_in_banquise(banquise, temp) == 0 // Le joueur ne doit pas apparaitre sur une case déjà occupée par un objet, un autre joueur, ou de l'eau
            || banquise.tab[temp.posx][temp.posy].type_case != glace
            || banquise.tab[temp.posx][temp.posy].but != defaut
            || banquise.tab[temp.posx][temp.posy].joueur != NULL);
    return temp;
}

/* Initialise la liste des joueurs à partir des données d'initialisation du jeu et de la banquise
 * Complexité O(n)
 *Complexité = O(nbJoueurs)
*/
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
        joueur[i].piege = 0;
        joueur[i].estEnVie = 1;
        banquise.tab[pos_joueur.posx][pos_joueur.posy].joueur = &joueur[i];
    }
    return joueur;
}
/*Permet de creer la structure de la banquise: case=eau/case=glace
 *Complexité:
*/
/* Créé un tableau de int contenant 1 si la case est de type 'eau' et adjascente a au moins une case de type 'glace'
 * Utilisé pour améliorer la structure de la génération de la banquise
 * */
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
/*Affiche pour chaque joueur du jeu sa position,son vecteur, son score
 * si il est sur un piege et si il est envie et le nombre de tours
 *Complexité: est O(nb_joueur)
*/
/* Affiche l'état des joueurs (position, vecteurs, piège, score, s'il est en vie, et le nombre total de tours)
 * Uniquement affiché pour debug
 * */
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
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        fprintf(stdout, "piege : %d\t\t", banquise.joueurs[i].piege);
    }
    fprintf(stdout, "\n");
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        fprintf(stdout, "score : %d %d %d\t\t", banquise.joueurs[i].score.distance,
                banquise.joueurs[i].score.nb_victime, banquise.joueurs[i].score.nb_glacon);
    }
    fprintf(stdout, "\n");
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        fprintf(stdout, "En vie : %d \t\t", banquise.joueurs[i].estEnVie);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "tours : %d\n", banquise.tours);
}
/* affiche les attributs de chaque case : type, but et objet.
 * Complexité : 0(taille*taille)
*/
void debug_case(T_banquise banquise, T_pos pos) {
    int type = 0, but = 0, objet = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (pos.posx + i < banquise.taille && pos.posx + i > 0 && pos.posy + j > 0 && pos.posy + j < banquise.taille) {
                T_case c = banquise.tab[pos.posx + i][pos.posy + j];
                type = c.type_case;
                but = c.but;
                objet = c.objet;
                fprintf(stdout, "%d %d : Type : %d But : %d Objet : %d\n", pos.posx + i, pos.posy + j, type, but, objet);
            }
        }
    }
}
/*dit si la partie est fini on non en fonction du nombre de joueur encore vivant
 * Complexité : O(nombre_joueur)
*/
bool is_partie_finie(T_banquise banquise) {
    T_pos arrive = position_arrive(banquise);
    int compteur = banquise.nombre_joueur;
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        if (banquise.joueurs[i].estEnVie == 0) {
            compteur -= 1;
        }
    }
    if (banquise.tab[arrive.posx][arrive.posy].joueur == NULL && compteur != 0) {
        return true;
    } else {
        return false;
    }
}
/* dit si un char en entrée est un vecteur de mouvement
 * Complexité : Constante
*/
bool is_vec(char input) {
    if (input == 'q' || input == 'z' || input == 's' || input == 'd') {
        return TRUE;
    } else {
        return FALSE;
    }
}
/*modifie l'emplacement du joueur en prenant en compte les différents attributs et obstacles présents sur la banquise
 *Complexité : constante
*/
void gestion_joueur(T_banquise banquise, int ID_joueur) {
    printf("Joueur %d input :\n", ID_joueur + 1);
    char input;
    fflush(stdin);
    scanf("%c", &input);
    if (is_vec(input)) {
        T_vec vec = char_to_t_vec(input);
        banquise.joueurs[ID_joueur].vecteur = vec;
        T_joueur joueur = banquise.joueurs[ID_joueur];
        T_pos pos_joueur = joueur.position;
        T_vec vec_joueur = joueur.vecteur;
        if (banquise.joueurs[ID_joueur].piege == 1) {
            banquise.joueurs[ID_joueur].piege = 0;
        } else if (pos_j_valide(banquise, ID_joueur) == 1) {
             if(banquise.tab[pos_joueur.posx + vec_joueur.dx][pos_joueur.posy + vec_joueur.dy].type_case==eau){
                banquise.tab[pos_joueur.posx][pos_joueur.posy].joueur= NULL;
                banquise.joueurs[ID_joueur].estEnVie=0;
            }else{
            switch (banquise.tab[pos_joueur.posx + vec_joueur.dx][pos_joueur.posy + vec_joueur.dy].objet) {
                case 0: // glacon
                    banquise.joueurs[ID_joueur].score.nb_glacon += 1;
                    move_glacon(banquise, ID_joueur);
                    // deplacer_joueur(banquise, ID_joueur);
                    break;
                case 1: // resort
                    break;
                case 2: // marteau_tete
                    break;
                case 3: // marteau_manche
                    break;
                case 4: // rocher
                    break;
                case 5: // piege
                    piege_joueur(banquise, ID_joueur);
                    banquise.joueurs[ID_joueur].score.distance += 1;
                    deplacer_joueur(banquise, ID_joueur);
                    break;
                default: // 6 - vide
                    banquise.joueurs[ID_joueur].score.distance += 1;
                    deplacer_joueur(banquise, ID_joueur);
                    break;
            }
       } } else {
            affiche_banquise(banquise);
            gestion_joueur(banquise, ID_joueur);
        }
    } else if (input == 'f') {
        printf("\nDirection pour placer le rocher\n");
        char input_vec;
        fflush(stdin);
        scanf("%c", &input_vec);
        T_vec vec = char_to_t_vec(input_vec);
        banquise.joueurs[ID_joueur].vecteur = vec;
        place_rocher(banquise, ID_joueur);
    }
}
/*bloque le joueur sur une case pendant un tour si c'est un piege
 *Complexité: constante
*/
void piege_joueur(T_banquise banquise, int ID_joueur) {
    T_pos pos = banquise.joueurs[ID_joueur].position;
    T_vec vec = banquise.joueurs[ID_joueur].vecteur;
    T_pos new_pos;
    new_pos.posx = pos.posx + vec.dx;
    new_pos.posy = pos.posy + vec.dy;
    banquise.tab[new_pos.posx][new_pos.posy].objet = vide;
    banquise.joueurs[ID_joueur].piege = 1;
}
/*Permet à un joueur de placer un rocher sur la banquise
 *Complexité : Constante
*/
void place_rocher(T_banquise banquise, int ID_joueur) {
    T_pos pos = banquise.joueurs[ID_joueur].position;
    T_vec vec = banquise.joueurs[ID_joueur].vecteur;
    T_pos pos_rocher;
    pos_rocher.posx = pos.posx + vec.dx;
    pos_rocher.posy = pos.posy + vec.dy;
    banquise.joueurs[ID_joueur].vecteur.dx = 0;
    banquise.joueurs[ID_joueur].vecteur.dy = 0;
    T_case case_rocher = banquise.tab[pos_rocher.posx][pos_rocher.posy];
    if (case_rocher.joueur == NULL && case_rocher.objet == vide && case_rocher.type_case == glace &&
        case_rocher.but == defaut) {
        banquise.tab[pos_rocher.posx][pos_rocher.posy].objet = rocher;
    } else if (case_rocher.joueur == NULL && case_rocher.objet == rocher && case_rocher.type_case == glace &&
               case_rocher.but == defaut) {
        banquise.tab[pos_rocher.posx][pos_rocher.posy].objet = vide;
    }
}
/* Calucule le score d'un joueur
 * Complexité : constante
*/
int calculer_score(T_banquise banquise, int ID_joueur) {
    if (banquise.joueurs[ID_joueur].estEnVie == 1) {
        return 20 * banquise.joueurs[ID_joueur].score.nb_victime + 5 * banquise.joueurs[ID_joueur].score.nb_glacon +
               banquise.joueurs[ID_joueur].score.distance + 40;
    } else {
        return 20 * banquise.joueurs[ID_joueur].score.nb_victime + 5 * banquise.joueurs[ID_joueur].score.nb_glacon +
               banquise.joueurs[ID_joueur].score.distance;
    }
}
/* Donne l'ordre des joueurs selon leurs score
 * Complexité : O(nombre_joueur)
*/
int *classer_joueur(T_banquise banquise) {
    int *scores;
    int *IDs;
    scores = (int *) malloc(banquise.nombre_joueur * sizeof(int));
    IDs = (int *) malloc(banquise.nombre_joueur * sizeof(int));
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        scores[i] = calculer_score(banquise, i);
        IDs[i] = i;
    }
    for (int i = 0; i < banquise.nombre_joueur; i++) {
        for (int j = i; j < banquise.nombre_joueur; j++) {
            if (scores[i] < scores[j]) {
                int temp_score = scores[i], temp_ID = IDs[i];
                scores[i] = scores[j];
                IDs[i] = IDs[j];
                scores[j] = temp_score;
                IDs[j] = temp_ID;
            }
        }
    }
    return IDs;
}
/* Fait tourner un marteau 3/4 de tour
 * Complexité : constante
*/
void tourner_marteau(T_banquise banquise, int sens, T_pos pos_marteau_manche) {
    int i = pos_marteau_manche.posx, j = pos_marteau_manche.posy;
    if (sens == 0) {
        if (banquise.tab[i + 1][j].objet == marteau_tete && banquise.tab[i][j + 1].joueur == NULL) {
            banquise.tab[i + 1][j].objet = vide;
            banquise.tab[i][j + 1].objet = marteau_tete;
        } else if (banquise.tab[i - 1][j].objet == marteau_tete && banquise.tab[i][j - 1].joueur == NULL) {
            banquise.tab[i - 1][j].objet = vide;
            banquise.tab[i][j - 1].objet = marteau_tete;
        } else if (banquise.tab[i][j + 1].objet == marteau_tete && banquise.tab[i - 1][j].joueur == NULL) {
            banquise.tab[i][j + 1].objet = vide;
            banquise.tab[i - 1][j].objet = marteau_tete;
        } else if (banquise.tab[i][j - 1].objet == marteau_tete && banquise.tab[i + 1][j].joueur == NULL) {
            banquise.tab[i][j - 1].objet = vide;
            banquise.tab[i + 1][j].objet = marteau_tete;
        }
    } else if (sens == 1) {
        if (banquise.tab[i + 1][j].objet == marteau_tete && banquise.tab[i][j - 1].joueur == NULL) {
            banquise.tab[i + 1][j].objet = vide;
            banquise.tab[i][j - 1].objet = marteau_tete;
        } else if (banquise.tab[i - 1][j].objet == marteau_tete && banquise.tab[i][j + 1].joueur == NULL) {
            banquise.tab[i - 1][j].objet = vide;
            banquise.tab[i][j + 1].objet = marteau_tete;
        } else if (banquise.tab[i][j + 1].objet == marteau_tete && banquise.tab[i + 1][j].joueur == NULL) {
            banquise.tab[i][j + 1].objet = vide;
            banquise.tab[i + 1][j].objet = marteau_tete;
        } else if (banquise.tab[i][j - 1].objet == marteau_tete && banquise.tab[i - 1][j].joueur == NULL) {
            banquise.tab[i][j - 1].objet = vide;
            banquise.tab[i - 1][j].objet = marteau_tete;
        }
    }
}
/*
 *Complexité : Constante
*/
int get_rotation_marteau(T_banquise banquise, T_pos pos_marteau_tete, T_pos pos_marteau_manche, T_pos new_pos_marteau_tete) {
    int i = pos_marteau_manche.posx, j = pos_marteau_manche.posy;
    T_pos result;
    if (banquise.tab[i + 1][j].objet == marteau_tete && banquise.tab[i][j + 1].objet == vide && banquise.tab[i][j + 1].joueur == NULL) {
        result.posx = i;
        result.posy = j + 1;
    } else if (banquise.tab[i - 1][j].objet == marteau_tete && banquise.tab[i][j - 1].objet == vide && banquise.tab[i][j - 1].joueur == NULL) {
        result.posx = i;
        result.posy = j - 1;
    } else if (banquise.tab[i][j + 1].objet == marteau_tete && banquise.tab[i - 1][j].objet == vide && banquise.tab[i - 1][j].joueur == NULL) {
        result.posx = i - 1;
        result.posy = j;
    } else if (banquise.tab[i][j - 1].objet == marteau_tete && banquise.tab[i + 1][j].objet == vide && banquise.tab[i + 1][j].joueur == NULL) {
        result.posx = i + 1;
        result.posy = j;
    }
    if (new_pos_marteau_tete.posx == result.posx && new_pos_marteau_tete.posy == result.posy) {
        return 0;
    } else {
        return 1;
    }
}
/*
 *Complexité : constante
*/
T_vec vector_marteau(T_banquise banquise, T_pos pos_glacon, T_pos pos_marteau_tete) {
    T_vec vec;
    vec.dx = pos_glacon.posx - pos_marteau_tete.posx;
    vec.dy = pos_glacon.posy - pos_marteau_tete.posy;
    return vec;
}
/*cherche le manche en fonction de la tete du marteau
 * Complexité : Constante
*/
T_pos trouver_marteau_manche(T_banquise banquise, T_pos pos_marteau_tete) {
    T_pos result;
    if (banquise.tab[pos_marteau_tete.posx + 1][pos_marteau_tete.posy].objet == marteau_manche) {
        result.posx = pos_marteau_tete.posx + 1;
        result.posy = pos_marteau_tete.posy;
        return result;
    } else if (banquise.tab[pos_marteau_tete.posx - 1][pos_marteau_tete.posy].objet == marteau_manche) {
        result.posx = pos_marteau_tete.posx - 1;
        result.posy = pos_marteau_tete.posy;
        return result;
    } else if (banquise.tab[pos_marteau_tete.posx][pos_marteau_tete.posy + 1].objet == marteau_manche) {
        result.posx = pos_marteau_tete.posx;
        result.posy = pos_marteau_tete.posy + 1;
        return result;
    } else if (banquise.tab[pos_marteau_tete.posx][pos_marteau_tete.posy - 1].objet == marteau_manche) {
        result.posx = pos_marteau_tete.posx;
        result.posy = pos_marteau_tete.posy - 1;
        return result;
    } else return pos_marteau_tete;
}
/* cherche la position de la tete en fonction de la position du manche
 * Complecité: Constante
*/
T_pos trouver_marteau_tete(T_banquise banquise, T_pos pos_marteau_manche) {
    T_pos result;
    if (banquise.tab[pos_marteau_manche.posx + 1][pos_marteau_manche.posy].objet == marteau_tete) {
        result.posx = pos_marteau_manche.posx + 1;
        result.posy = pos_marteau_manche.posy;
        return result;
    } else if (banquise.tab[pos_marteau_manche.posx - 1][pos_marteau_manche.posy].objet == marteau_tete) {
        result.posx = pos_marteau_manche.posx - 1;
        result.posy = pos_marteau_manche.posy;
        return result;
    } else if (banquise.tab[pos_marteau_manche.posx][pos_marteau_manche.posy + 1].objet == marteau_tete) {
        result.posx = pos_marteau_manche.posx;
        result.posy = pos_marteau_manche.posy + 1;
        return result;
    } else if (banquise.tab[pos_marteau_manche.posx][pos_marteau_manche.posy - 1].objet == marteau_tete) {
        result.posx = pos_marteau_manche.posx;
        result.posy = pos_marteau_manche.posy - 1;
        return result;
    } else return pos_marteau_manche;
}

/* Code Ines */
/* Cree l'espace mémoire du tableau
 * Complexité :O(n)
 */
T_case **alloue(int n) {
    int h;
    T_case **tab = (T_case **) malloc(n * sizeof(T_case *));
    for (h = 0; h < n; h++) {
        tab[h] = (T_case *) malloc(n * sizeof(T_case));
    }
    return tab;
}
/* Remplie des case du tableau de [x-1,y-1] a [x+1,y+1] de glace
 * Complexité :O(taille*taille)
 */
void remp_banquise_tab_aux(T_case **tab, int taille, int x, int y) {
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i < taille && i >= 0 && j < taille && j >= 0) {
                tab[i][j].type_case = glace; // La glace est représenté par * dans la console
            }
        }
    }
}
/* Remplit par defaut toute les cases du tableau est rajoute de la glace avec remp_banquise_tab_aux
 * Complexité : O(taille*taille)
*/
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
/* initialise la case de départ alétoirement
 *Complexité
*/
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
/* initialise la case d'arrive aléatoirement
 *Complexité :
*/
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
/*change la position d'un joueur selon la valeur de son vecteur
 *Complexité :Constante
*/
void move_j_aux(T_banquise banquise, int joueur) {
    T_pos pos = banquise.joueurs[joueur].position;
    T_vec vec = banquise.joueurs[joueur].vecteur;
    T_pos new_pos;
    new_pos.posx = pos.posx + vec.dx;
    new_pos.posy = pos.posy + vec.dy;
    banquise.tab[new_pos.posx][new_pos.posy].joueur = &banquise.joueurs[joueur];
}

/*dire si la nouvelle case du joueur est valide
 *Complexité : constante
*/
int pos_j_valide(T_banquise banquise, int joueur) {
    int i = banquise.joueurs[joueur].position.posx + banquise.joueurs[joueur].vecteur.dx;
    int j = banquise.joueurs[joueur].position.posy + banquise.joueurs[joueur].vecteur.dy;
    if (i < 0 || i >= banquise.taille || j < 0 || j >= banquise.taille) {
        return 0;
    } else if (banquise.tab[i][j].joueur != NULL) {
        return 0;
    } else {
        return 1;
    }
}

/* quand un joueur n'est plus sur une case valide il ne doit plus etre sur la banquise.
 * Complexité: constante
*/
void mettre_case_j_null(T_banquise banquise, int joueur) {
    T_pos pos = banquise.joueurs[joueur].position;
    banquise.tab[pos.posx][pos.posy].joueur = NULL;
}

/*faire bouger un joueur d'une case à l'autre
 * Complexité : constante
*/
void deplacer_joueur(T_banquise banquise, int joueur) {
    T_pos pos_joueur = banquise.joueurs[joueur].position;
    T_vec vec_joueur = banquise.joueurs[joueur].vecteur;
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
/* change les cases de glace qui sont proche de l'eau en des cases d'eau
 * et fait aussi fondre des glacons
*/
void rechauffement_climatique(T_banquise banquise) {
    int **search = create_tab_chemin(banquise.taille);
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            search[i][j] = 0;
            if (banquise.tab[i][j].type_case == 1 && banquise.tab[i][j].objet == 6 && banquise.tab[i][j].joueur == NULL && banquise.tab[i][j].but == 2) {
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
                } else {
                    search[i][j] = 0;
                }
            } else {
                search[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            if (search[i][j] == 1 && (rand() % 1000 <= FONTE_GLACE)) {
                banquise.tab[i][j].type_case = 0;
            }
        }
    }
    for (int i = 0; i < banquise.taille; i++) {
        for (int j = 0; j < banquise.taille; j++) {
            if (banquise.tab[i][j].objet == glacon && (rand() % 1000 <= FONTE_GLACE)) {
                banquise.tab[i][j].objet = vide;
            }
        }
    }
}

/* prend en charge les mouvements du glaçon
 * Complexité en O(taille)
*/
void move_glacon(T_banquise banquise, int joueur) {
    T_pos pos = banquise.joueurs[joueur].position;
    T_vec vec = banquise.joueurs[joueur].vecteur;
    T_pos new_pos;
    pos.posx = pos.posx + vec.dx;
    pos.posy = pos.posy + vec.dy;
    new_pos.posx = pos.posx + vec.dx;
    new_pos.posy = pos.posy + vec.dy;

    while (new_pos.posx >= 0 && new_pos.posx < banquise.taille && new_pos.posy >= 0 && new_pos.posy < banquise.taille) {
        affiche_banquise(banquise);
        T_case new_case = banquise.tab[new_pos.posx][new_pos.posy];
        if (new_case.joueur != NULL) {
            banquise.tab[new_pos.posx][new_pos.posy].joueur->estEnVie = 0;
            banquise.tab[new_pos.posx][new_pos.posy].joueur = NULL;
            banquise.tab[pos.posx][pos.posy].objet = vide;
            banquise.joueurs[joueur].score.nb_victime += 1;
            break;
        } else if (new_case.objet == vide && new_case.type_case == glace) {
            banquise.tab[new_pos.posx][new_pos.posy].objet = glacon;
            banquise.tab[pos.posx][pos.posy].objet = vide;
            pos = new_pos;
            new_pos.posx = new_pos.posx + vec.dx;
            new_pos.posy = new_pos.posy + vec.dy;
        } else if (new_case.type_case == eau) {
            banquise.tab[new_pos.posx][new_pos.posy].type_case = glace;
            banquise.tab[pos.posx][pos.posy].objet = vide;
            break;
        } else if (new_case.objet == resort) {
            vec.dx = -1 * vec.dx;
            vec.dy = -1 * vec.dy;
            new_pos.posx = pos.posx + vec.dx;
            new_pos.posy = pos.posy + vec.dy;
        } else if (new_case.objet == rocher || new_case.objet == piege  || new_case.objet == glacon  ) {
            break;
        } else if (new_case.objet == marteau_tete){
            T_pos temp = new_pos;
            temp.posx = temp.posx + vec.dx;
            temp.posy = temp.posy + vec.dy;
            if (banquise.tab[temp.posx][temp.posy].objet == marteau_manche) {
                break;
            } else {
                T_pos pos_marteau_tete = new_pos;
                T_pos pos_marteau_manche;
                pos_marteau_manche = trouver_marteau_manche(banquise, pos_marteau_tete);
                T_pos new_pos_marteau_tete;
                new_pos_marteau_tete.posx = pos_marteau_manche.posx + vec.dx;
                new_pos_marteau_tete.posy = pos_marteau_manche.posy + vec.dy;
                int rotation = get_rotation_marteau(banquise, pos_marteau_tete, pos_marteau_manche, new_pos_marteau_tete);
                for (int i = 0; i < 3; i++) {
                    tourner_marteau(banquise, rotation, pos_marteau_manche);
                    affiche_banquise(banquise);
                }
                pos_marteau_tete = trouver_marteau_tete(banquise, pos_marteau_manche);
                T_vec old_vec = vec;
                vec = vector_marteau(banquise, pos, pos_marteau_tete);
                new_pos.posx = new_pos.posx + vec.dx - old_vec.dx;
                new_pos.posy = new_pos.posy + vec.dy - old_vec.dy;
            }
        } else {
            break;
        }
    }
}
/* initialise les glaçons sur la glace
 * Complexité:
*/
void init_glacon(T_case **tab, int taille) {
    int i, j;

    for (int l = 0; l < NB_GLACON; l++) {
        do {
            i = rand() % taille;
            j = rand() % taille;
        } while (tab[i][j].type_case != 1 && tab[i][j].objet != vide && tab[i][j].joueur != NULL && tab[i][j].but != defaut);
        tab[i][j].objet = glacon;
    }

}
/* initialise les ressorts sur la glace
 *Complexité
*/
void init_ressort(T_case **tab, int taille) {
    int i, j;
    for (int l = 0; l < 2 + taille / 10; l++) {
        do {
            i = rand() % taille;
            j = rand() % taille;
        } while (tab[i][j].type_case != 1 && tab[i][j].objet != vide && tab[i][j].joueur != NULL &&
                 tab[i][j].but != defaut);
        tab[i][j].objet = resort;
    }
}
/* initialise les pieges sur la glace
 *Complexité
*/
void init_piege(T_case **tab, int taille) {
    int i, j;
    for (int l = 0; l < 2 + taille / 10; l++) {
        do {
            i = rand() % taille;
            j = rand() % taille;
        } while (tab[i][j].type_case != 1 && tab[i][j].objet != vide && tab[i][j].joueur != NULL &&
                 tab[i][j].but != defaut);
        tab[i][j].objet = piege;
    }
}
/* initialise les rochers
 * Complexité :
*/
void init_rocher(T_case **tab, int taille) {
    int i, j;
    for (int l = 0; l < 2 + taille / 10; l++) {
        do {
            i = rand() % taille;
            j = rand() % taille;
        } while (tab[i][j].type_case != 1 && tab[i][j].objet != vide && tab[i][j].joueur != NULL &&
                 tab[i][j].but != defaut);
        tab[i][j].objet = rocher;
    }
}
/* initialise le marteau
 * Complexité :
*/
void init_marteau(T_case **tab, int taille) {
    int i, j;
    for (int l = 0; l < 1 + taille / 10; l++) {
        do {
            i = rand() % (taille - 3) + 1;
            j = rand() % (taille - 3) + 1;
        } while (tab[i][j].type_case != 1 && tab[i][j].objet != 6 && tab[i][j].joueur != NULL && tab[i][j].but != 2
                 && tab[i + 1][j].type_case != 1 && tab[i + 1][j].objet != 6 && tab[i + 1][j].joueur != NULL && tab[i + 1][j].but != 2
                 && tab[i - 1][j].type_case != 1 && tab[i - 1][j].objet != 6 && tab[i - 1][j].joueur != NULL && tab[i - 1][j].but != 2
                 && tab[i][j + 1].type_case != 1 && tab[i][j + 1].objet != 6 && tab[i][j + 1].joueur != NULL && tab[i][j + 1].but != 2
                 && tab[i][j - 1].type_case != 1 && tab[i][j - 1].objet != 6 && tab[i][j - 1].joueur != NULL && tab[i][j - 1].but != 2
                );
        tab[i][j].objet = marteau_manche;
        tab[i][j + 1].objet = marteau_tete;
    }
}

/* Permet d'avoir un affichage en couleur
 * Complexité: Constante
*/
void Color(int couleurDuTexte, int couleurDeFond) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}
