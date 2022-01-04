#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lecture_entete.h"
#include "elf_header.h"


int main(int argc, char **argv) {

    if (argc < 3) {
        fprintf(stderr, "Nombre d'arguments incorrect\n");
        return EXIT_FAILURE;
    }

    char c = argv[1][0]; 

    if (c == 'a'){
        printf("\n\t\t\t-----MODE EMPLOI-----\n\n\th : Affichage de l'en-tete\n\tS : Affichage de la table des sections\n\tx : Affichage du contenu d'une section\n\ts : Affichage de la table des symboles\n\tr : Affichage des tables de reimplantation\n");
    }
    switch (c)
    {
    case 'h':
        lire_entete(argv[2]);
        break;

    case 'S':
        printf("TO DO");
        break;
    
    case 'x':
        printf("TO DO");
        break;
    
    case 's':
        printf("TO DO");
        break;
    
    case 'r':
        printf("TO DO");
        break;
    
    default:
        break;
    }

    return EXIT_SUCCESS;
}
