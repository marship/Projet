#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

#include "elf_header.h"
#include "section_header.h"

char *lire_strtab(FILE *f, Elf32_Shdr *shdr, Elf32_Ehdr ehdr);

char *lire_shstrtab(FILE *f, Elf32_Shdr *shdr, Elf32_Ehdr ehdr);

void afficher_chaine(char *strtab, Elf32_Word index);

#endif