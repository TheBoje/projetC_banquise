#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "joueur.h"
#include "objet.h"


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
