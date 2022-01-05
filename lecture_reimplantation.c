#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lecture_reimplantation.h"
#include "fonctions_utilitaires.h"


Elf32_Rel *lire_rel(char *nom_fichier, Elf32_Ehdr ehdr, Elf32_Shdr *shdr) {
    FILE *f;
    Elf32_Rel *rel = malloc(ehdr.e_shnum * sizeof(Elf32_Rel));

    // Ouverture du fichier
    f = fopen(nom_fichier, "rb");

    if (f == NULL) {
        fprintf(stderr, "Error: Can't open file %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_REL) {
            if (fseek(f, shdr[0].sh_addr + shdr[0].sh_offset, SEEK_SET) != 0) {
                fprintf(stderr, "Erreur de lecture du fichier %s\n", nom_fichier);
                exit(EXIT_FAILURE);
            }

            if (read_uint32(&rel[i].r_offset, f, ehdr.e_ident[EI_DATA]) == 0
            || read_uint32(&rel[i].r_info, f, ehdr.e_ident[EI_DATA]) == 0) {
                fprintf(stderr, "Error: %s: Failed to read file\n", nom_fichier);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(f);

    return rel;
}

Elf32_Rela *lire_rela(char *nom_fichier, Elf32_Ehdr ehdr, Elf32_Shdr *shdr) {
    FILE *f;
    Elf32_Rela *rela = malloc(ehdr.e_shnum * sizeof(Elf32_Rela));

    // Ouverture du fichier
    f = fopen(nom_fichier, "rb");

    if (f == NULL) {
        fprintf(stderr, "Error: Can't open file %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_RELA) {
            if (fseek(f, shdr[0].sh_addr + shdr[0].sh_offset, SEEK_SET) != 0) {
                fprintf(stderr, "Erreur de lecture du fichier %s\n", nom_fichier);
                exit(EXIT_FAILURE);
            }

            if (read_uint32(&rela[i].r_offset, f, ehdr.e_ident[EI_DATA]) == 0
            || read_uint32(&rela[i].r_info, f, ehdr.e_ident[EI_DATA]) == 0
            || read_int32(&rela[i].r_addend, f, ehdr.e_ident[EI_DATA]) == 0) {
                fprintf(stderr, "Error: %s: Failed to read file\n", nom_fichier);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(f);

    return rela;
}

void afficher_reimplantations(Elf32_Rel *rel, Elf32_Rela *rela, Elf32_Ehdr ehdr, Elf32_Shdr *shdr) {
    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_REL) {

        }
        else if (shdr[i].sh_type == SHT_RELA) {

        }
    }
}