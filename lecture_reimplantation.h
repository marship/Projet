#ifndef __LECTURE_REIMPLANTATION_H__
#define __LECTURE_REIMPLANTATION_H__

#include "elf_relocation.h"
#include "elf_header.h"
#include "section_header.h"
#include "elf_symbol.h"

typedef union {
    Elf32_Rel *rel;
    Elf32_Rela *rela;
} Relocations;


Relocations *lire_relocations(char *nom_fichier, Elf32_Ehdr ehdr, Elf32_Shdr *shdr);

void liberer_relocations(Relocations *reloc, Elf32_Ehdr ehdr);

void afficher_relocations(Relocations *reloc, Elf32_Ehdr ehdr, Elf32_Shdr *shdr, char *shstrtab, Elf32_Sym *sym);


#endif
