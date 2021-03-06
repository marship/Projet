#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "section.h"
#include "fonctions_utilitaires.h"
#include "string_table.h"


unsigned char *lire_section(FILE *f, Elf32_Shdr *shdr, int num)
{
    if (shdr == NULL) {
        return NULL;
    }

    unsigned char *sect = malloc(shdr[num].sh_size * sizeof(char));

    if (sect == NULL) {
        fprintf(stderr, "ERREUR: Impossible d'allouer de la mémoire pour la section\n");
        exit(EXIT_FAILURE);
    }

    if (fseek(f, shdr[num].sh_offset, SEEK_SET) != 0) {
        fprintf(stderr,
                "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour la section\n",
                shdr[num].sh_size);
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < shdr[num].sh_size; i++) {
        if(fread(&sect[i], sizeof(char), 1, f) == 0) {
            fprintf(stderr,
                    "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour la section\n",
                    shdr[num].sh_size);
            exit(EXIT_FAILURE);
        }
    }
    return sect;
}


void afficher_section(unsigned char *sect, char *shstrtab, Elf32_Shdr *shdr, uint32_t index, Elf32_Half shnum)
{
    if (sect == NULL || shdr == NULL) {
        fprintf(stderr, "AVERTISSEMENT: La section n'a pas été vidangée parce qu'inexistante !");
        exit(EXIT_FAILURE);
    }

    if (shdr[index].sh_size == 0) {
        printf("Section '");
        afficher_chaine(shstrtab, shdr[index].sh_name, 0);
        printf("' has no data to dump.\n");
        return;
    }

    uint32_t i = 0;
    int j = 0;
    unsigned char ligne[17];

    printf("\nHex dump of section '");
    afficher_chaine(shstrtab, shdr[index].sh_name, 0);
    printf("':\n");

    for (uint32_t k = 0; k < shnum; k++) {
        if (shdr[k].sh_info == index && (shdr[k].sh_type == SHT_REL || shdr[k].sh_type == SHT_RELA)) {
            printf(" NOTE: This section has relocations against it, but these have NOT been applied to this dump.\n");
            break;
        }
    }

    while (i < shdr[index].sh_size) {
        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }

        if (i % 16 == 0) {
            if (i != 0) {
                printf ("%s\n", ligne);
            }
            while (j != 17) {
                ligne[j] = '\0';
                j++;
            }
            j = 0;
            printf ("  0x%.8x ", i + shdr[index].sh_addr);
        }

        printf("%.2x", sect[i]);

        if ((sect[i] < 0x20) || (sect[i] > 0x7e)) {
            ligne[i % 16] = '.';
        }
        else {
            ligne[i % 16] = sect[i];
        }
        ligne[(i % 16) + 1] = '\0';
        i++;
    }

    while ((i % 16) != 0) {
        if (i % 4 == 0) {
            printf(" ");
        }
        printf ("  ");
        i++;
    }
    printf (" %s\n\n", ligne);
}