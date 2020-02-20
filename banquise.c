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
void choisir_case_depart(T_case **tab, int n){
    int a = 0;
    int i , j ;
    srand(time(NULL));
    while (a==0){
    i = rand()%n;
    j = rand()%n;
    if(tab[i][j].type_case == glace && tab[i][j].but!= arrive){
        tab[i][j].but= depart;
        a=a+1;
    }

    }
}

void choisir_case_arrive(T_case **tab, int n){

  int a = 0;
    int i , j ;
    srand(time(NULL));
    while (a==0){
    i = rand()%n;
    j = rand()%n;
    if(tab[i][j].type_case == glace && tab[i][j].but != depart){
        tab[i][j].but= arrive;
        a=a+1;
    }

    }

}
