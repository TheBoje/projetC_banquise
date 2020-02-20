#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"

/* Code Louis */

T_case **create_tab(int taille)
{
    T_case **tab = alloue(taille);
    remp_banquise_tab(tab, taille);
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

void remp_banquise_tab(T_case **tab, int taille){
    int i,j;
    srand(time(NULL));
    for(i = 0 ; i < taille ; i++ ){
        for (j = 0 ; j < taille ; j++ ){
            tab[i][j].type_case = rand()%2;
        }
    }
}

void choisir_case_depart(T_case **tab, int n) {
    srand(time(NULL));
    int i = rand()%n;
    int j = rand()%n;
    while (tab[i][j].type_case != glace) {
        i = rand()%n;
        j = rand()%n;
    }
    tab[i][j].but = depart;
}

void choisir_case_arrive(T_case **tab, int n) {
    srand(time(NULL));
    int i = rand()%n;
    int j = rand()%n;
    while (tab[i][j].type_case != glace && tab[i][j].but != defaut) {
        i = rand()%n;
        j = rand()%n;
    }
    tab[i][j].but = arrive;
}

