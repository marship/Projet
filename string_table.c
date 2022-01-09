#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "string_table.h"


char *lire_string_table(FILE *f, Elf32_Addr addr, Elf32_Word size) {
    if (fseek(f, addr, SEEK_SET) != 0) {
        fprintf(stderr,
                "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour la string table\n",
                addr);
        exit(EXIT_FAILURE);
    }

    char *strtab = malloc(size * sizeof(char));

    for (Elf32_Word j = 0; j < size; j++) {
        if (fread(&strtab[j], sizeof(char), 1, f) == 0) {
            fprintf(stderr,
                    "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour la string table\n",
                    addr);
            exit(EXIT_FAILURE);
        }
    }

    return strtab;
}

char *lire_strtab(FILE *f, Elf32_Shdr *shdr, Elf32_Ehdr ehdr) {
    for (Elf32_Half i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_STRTAB && i != ehdr.e_shstrndx) {
            return lire_string_table(f, shdr[i].sh_addr + shdr[i].sh_offset, shdr[i].sh_size);
        }
    }
    return NULL;
}

char *lire_shstrtab(FILE *f, Elf32_Shdr *shdr, Elf32_Ehdr ehdr) {
    int i = ehdr.e_shstrndx;

    if (shdr[i].sh_type == SHT_STRTAB) {
        return lire_string_table(f, shdr[i].sh_addr + shdr[i].sh_offset, shdr[i].sh_size);
    }
    return NULL;
}

void afficher_chaine(char *strtab, Elf32_Word index) {
    if (strtab == NULL) {
        fprintf(stderr, "Erreur: string table absente\n");
        exit(EXIT_FAILURE);
    }

    while (strtab[index] != '\0') {
        printf("%c", strtab[index]);
        index++;
    }
}
