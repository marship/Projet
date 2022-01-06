#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

#include "elf_header.h"
#include "section_header.h"

char *lire_strtab(Elf32_Shdr *shdr, Elf32_Ehdr ehdr, char *nom_fichier);

char *lire_shstrtab(Elf32_Shdr *shdr, Elf32_Ehdr ehdr, char *nom_fichier);

void afficher_chaine(char *strtab, Elf32_Word index);

#endif