#include <stdio.h>
#include <stdlib.h>
#include "objet.h"

char  T_objet_to_char(T_objet objet) {
    char result;
    switch (objet) {
    case 0: // glacon
        result = 'G';
        break;
    case 1: // resort
        result = 'X';
        break;
    case 2: // Marteau tete
        result = 'T';
        break;
    case 3: // Marteau manche
        result = 'M';
        break;
    case 4: // Rocher
        result = 'R';
        break;
    case 5: // Piege
        result = 'P';
        break;
    case 6: // Vide
        result = '0';
        break;
    default:
        result = '0';
        break;
    }
    return result;
}

