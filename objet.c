#include <stdio.h>
#include <stdlib.h>
#include "objet.h"

/* Code Louis */
/* transforme le type enumure Objet en char pour pivoir les afficher
 * Complexite constante
*/
char T_objet_to_char(T_objet objet) {
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
            result = '*';
            break;
        case 6: // Vide
            result = ' ';
            break;
        default:
            result = ' ';
            break;
    }
    return result;
}

/* Code Ines */

