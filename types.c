#include <stdio.h>
#include <stdlib.h>

typedef enum {
    eau = 0;
    glace,
    joueur,
    objet,
    depart,
    arrive
}T_case;

typedef struct {
    **T_case banquise;
    int tailleX;
    int tailleY;
} T_banquise;
