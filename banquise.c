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
