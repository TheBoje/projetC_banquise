#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "windows.h"
#include "banquise.h"


/* Code Louis */

T_case **create_tab(int taille)
{
    T_case **tab = alloue(taille);
    remp_banquise_tab(tab, taille);
    choisir_case_depart(tab, taille);
    choisir_case_arrive(tab, taille);
    return tab;
}

T_banquise create_banquise(int taille, int joueurs)
{
    T_banquise banquise;
    banquise.tab = create_tab(taille);
    banquise.taille = taille;
    banquise.nombre_joueur = joueurs;
    return banquise;
}

void modif_type_case(T_banquise banquise, int x, int y,T_type_case type_case){
    banquise.tab[x][y].type_case = type_case;
}

void modif_pos_joueur(T_banquise banquise, T_joueur joueur){
    banquise.tab[joueur.position.posx][joueur.position.posy].joueur = NULL;
    banquise.tab[joueur.position.posx + joueur.vecteur.dx][joueur.position.posy + joueur.vecteur.dy].joueur = &joueur;
}

char T_but_to_char(T_but objet)
{
    char result;
    switch (objet)
    {
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

char T_case_to_char(T_type_case c){
    char result;
    switch (c)
    {
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

T_case **alloue(int n)
{
    int h;
    T_case ** tab =(T_case**) malloc(n*sizeof(T_case*));
    for (h = 0 ; h < n ; h++)
    {
        tab[h] = (T_case*)malloc(n * sizeof(T_case));
    }
    return tab;
}

void remp_banquise_tab_aux(T_case **tab,int taille,int x,int y){
    for (int i = x - 1; i <= x + 1 ; i++){
        for (int j = y - 1; j <= y + 1; j++){
            if (i < taille && i >= 0 && j < taille && j >= 0){
                tab[i][j].type_case = eau;
            }
        }
    }
}

void remp_banquise_tab(T_case **tab, int taille){
    int i,j,x,y,r;
    srand(clock());
    r = rand()%(2 * taille) + taille;
    for (int k = 0; k < r; k++){
        x = rand()%taille;
        y = rand()%taille;
        remp_banquise_tab_aux(tab, taille, x, y);
    }
    for(i = 0 ; i < taille ; i++ ){
        for (j = 0 ; j < taille ; j++ ){
            tab[i][j].but = defaut;
            tab[i][j].objet = vide;
        }
    }
}

void choisir_case_depart(T_case **tab, int taille) {
    FILETIME  now; // Problemes de génération de nombres pseudos-aléatoires pour le placement des cases (seed actualisée toutes les secondes seulement), donc utilisation de l'heure systeme en microsecondes
    GetSystemTimeAsFileTime(&now);
    srand((unsigned long)&now);
    int i, j;
    i = rand() % taille;
    j = rand() % taille;
    if (tab[i][j].type_case == glace)
    {
        choisir_case_depart(tab, taille);
    }
    else
    {
        tab[i][j].but = depart;

    }
}

void choisir_case_arrive(T_case **tab, int taille) {
    FILETIME  now;
    GetSystemTimeAsFileTime(&now);
    srand((unsigned long)&now);
    int i, j;
    i = rand() % taille;
    j = rand() % taille;
    if (tab[i][j].type_case == glace || tab[i][j].but == depart)
    {
        choisir_case_arrive(tab, taille);
    }
    else
    {
        tab[i][j].but = arrive;
    }
}


