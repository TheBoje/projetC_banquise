#include <stdio.h>
#include <stdlib.h>
#include "banquise.h"

T_case **alloue(int n)
{
    int h;
    T_case ** tab =(T_case**)malloc(n*sizeof(T_case*));
    for (h = 0 ; h < n ; h++)
    {
        tab[h] = (T_case*)malloc(n * sizeof(T_case));
    }
    return tab;
}

void remp_banquise_tab(T_case **tab, int n)
{
    int i,j;
    srand(time(NULL));
    for(i=0; i<n;i++){
        for (j=0;j<n;j++){
            tab[i][j].type_case = 1;
        }
    }
}


void  affiche_banquise (T_case **tab, int n)
{
    int i,j;
    for(i=0; i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            fprintf(stdout, " %d " ,tab[i][j].type_case);
        }
    fprintf(stdout,"\n");
    }
}


int main()
{
    T_case tab = alloue(5);
    affiche_banquise(tab, 5);
    remp_banquise_tab(tab, 5);
    affiche_banquise(tab, 5);
    return 0;
}
