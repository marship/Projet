#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "aff_section.h"
#include "fonctions_utilitaires.h"
#include "string_table.h"

char *lire_une_section(char *nom_fichier, Elf32_Shdr *shdr, int num){
    FILE *f;
    char *sect = malloc(shdr[num].sh_size * sizeof(char));

    // Ouverture du fichier
    f = fopen(nom_fichier, "rb");

    if (f == NULL) {
        fprintf(stderr, "Error: Can't open file %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }
    if (fseek(f, shdr[num].sh_addr + shdr[num].sh_offset, SEEK_SET) != 0) {
        fprintf(stderr, "Erreur de lecture du fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    uint32_t i = 0;
    while(i != shdr[num].sh_size){
        if(fread(&sect[i], sizeof(char), 1, f) == 0){
            fprintf(stderr, "Erreur: %s: Echec de la lecteur du symbole %d\n", nom_fichier, i);
            exit(EXIT_FAILURE);
        }
        i++;
    }
    return sect;
}

void aff_une_section(char *sect, uint32_t nbChar){
    uint32_t i = 0;
    int j = 0;
    unsigned char ligne[17];
    printf("Hex Dump de la section :\n");
    printf("  Offset     Code Hexadecimal                      Traduction\n");
    while(i != nbChar){
        if(i % 4 == 0 && i != 0){
            printf(" ");
        }
        if(i % 16 == 0){
            if(i != 0 ){
                printf ("  %s\n", ligne);
            }
            while(j != 17){
                ligne[j] = ' ';
                j++;
            }
            j = 0;
            printf ("  0x%.8x ", i);
        }
        printf("%.2x", sect[i]);
        if ((sect[i] < 0x20) || (sect[i] > 0x7e))
            ligne[i % 16] = '.';
        else
            ligne[i % 16] = sect[i];
        ligne[(i % 16) + 1] = ' ';
        i++;
    }
    while ((i % 16) != 0) {
        if(i % 4 == 0){
            printf(" ");
        }
        printf ("  ");
        i++;
    }
    printf ("   %s\n", ligne);
}