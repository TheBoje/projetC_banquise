#include <stdio.h>
#include <stdlib.h>
#include "joueur.h"
#define CTRL_UP 122
#define CTRL_DOWN 115
#define CTRL_LEFT 113
#define CTRL_RIGHT 100

/* Code Louis */

/* Code Ines */

file_j newFile(){
    file_j f;
    f.tete=f.fin=NULL;
    return f;
}

void enfiler (file_j *f, T_joueur joueur)
{
    cellule *c = (cellule *)malloc(sizeof(cellule));
    c->joueur=joueur;
    if (f->tete==NULL)
    {
        c->suiv=NULL;
        f->tete = c;
        f->fin = c;
    } else
    {
        f->fin->suiv=c;
        f->fin = c;
        c->suiv=NULL;
    }
}

void defiler(file_j *f)
{
    cellule *c = f->tete;
    if(f->tete != NULL)
    {
        f->tete= f->tete->suiv;
        free(c);
        if(f->tete==NULL){
            f->fin=NULL;
        }
    }
}

file_j create_file_joueur(int joueurs){
    file_j f = newFile();
    for (int i =0;i<joueurs;i++){
        T_joueur j;
        enfiler(&f,j);
    }
    return f;
}
