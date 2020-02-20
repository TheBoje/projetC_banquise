#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "joueur.h"
#include "objet.h"


void  affiche_banquise (T_banquise banquise){
    int i,j;
    for(i = 0; i < banquise.taille; i++ ){
        for (j = 0; j < banquise.taille; j++ ){
            fprintf(stdout, " %d " ,banquise.tab[i][j].type_case);
        }
        fprintf(stdout,"\n");
    }
}


int main()
{
    T_banquise banquise = create_banquise(9, 1);
    affiche_banquise(banquise);

    return 0;
}
