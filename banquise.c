#include <stdio.h>
#include <stdlib.h>
#include "banquise.h"
#include <time.h>

void remp_banquise_tab(T_case **tab, int n){
   int i,j;
   srand(time(NULL));
    for(i=0; i<n;i++){
        for (j=0;j<n;j++){
       tab[i][j].type_case = rand()%2;
        }
    }
}

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

// a faire : T_case create_tab(int taille){}; retourne le tab final

T_banquise create_banquise(int taille, int joueurs) {
    T_banquise banquise;
    banquise.tab = create_tab(taille);
    banquise.taille = taille;
    banquise.nombre_joueur = joueurs;
    return T_banquise;
}
