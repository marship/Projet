#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "string_table.h"


char *lire_string_table(Elf32_Addr addr, Elf32_Word size, FILE *f, char *nom_fichier) {
    if (fseek(f, addr, SEEK_SET) != 0) {
        fprintf(stderr, "Erreur de lecture du fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    char *strtab = malloc(size * sizeof(char));

    for (int j = 0; j < size; j++) {
        if (fread(&strtab[j], sizeof(char), 1, f) == 0) {
            fprintf(stderr, "Erreur de lecture du fichier %s\n", nom_fichier);
            exit(EXIT_FAILURE);
        }
    }

    return strtab;
}


char *lire_strtab(Elf32_Shdr *shdr, Elf32_Ehdr ehdr, FILE *f, char *nom_fichier) {
    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_STRTAB && i != ehdr.e_shstrndx) {
            return lire_string_table(shdr[i].sh_addr + shdr[i].sh_offset, shdr[i].sh_size, f, nom_fichier);
        }
    }
    fprintf(stderr, "Erreur: string table absente\n");
    exit(EXIT_FAILURE);
}

char *lire_shstrtab(Elf32_Shdr *shdr, Elf32_Ehdr ehdr, FILE *f, char *nom_fichier) {
    int i = ehdr.e_shstrndx;
    return lire_chaine(shdr[i].sh_addr + shdr[i].sh_offset, shdr[i].sh_size, f, nom_fichier);
}

void afficher_chaine(char *strtab, Elf32_Word index) {
    while (strtab[index] != '\0') {
        printf("%c", strtab[index]);
    }
}
