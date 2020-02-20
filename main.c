#include <stdio.h>
#include <stdlib.h>
#include "banquise.h"
#include "joueur.h"
#include "objet.h"
#include <time.h>
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

void remp_banquise_tab(T_case **tab, int n){
   int i,j;
   srand(time(NULL));
    for(i=0; i<n;i++){
        for (j=0;j<n;j++){
       tab[i][j].type_case = rand()%2;
        }
    }

}

void  affiche_banquise (T_case **tab, int n){
int i,j;
 for(i=0; i<n;i++){
        for (j=0;j<n;j++){
          fprintf(stdout, " %d " ,tab[i][j].type_case);
        }
        fprintf(stdout,"\n");
    }



}










int main()
{

  T_case **t = alloue(9);
   remp_banquise_tab(t,9);
   affiche_banquise(t,9);







    return 0;
}
