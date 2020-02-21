#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"


/* Code Louis */

/* Code Ines */

void affiche_banquise(T_banquise banquise) {
    int i, j;
    for (i = 0; i < banquise.taille; i++) {
        for (j = 0; j < banquise.taille; j++) {
            if (banquise.tab[i][j].joueur != NULL) {
                fprintf(stdout, " %c ", 'X');
            } else if (banquise.tab[i][j].but != defaut) {
                fprintf(stdout, " %c ", T_but_to_char(banquise.tab[i][j].but));
            } else if (banquise.tab[i][j].objet != vide) {
                fprintf(stdout, " %c ", T_objet_to_char(banquise.tab[i][j].objet));
            } else {
                fprintf(stdout, " %c ", T_case_to_char(banquise.tab[i][j].type_case));
            }

        }
        fprintf(stdout, "\n");
    }
}


int main() {
    /* Code Louis */

    T_banquise banquise = create_banquise(20, 1);
    affiche_banquise(banquise);

    /* Code Ines */

    return 0;
}
