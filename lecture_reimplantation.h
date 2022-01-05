#ifndef __LECTURE_REIMPLANTATION_H__
#define __LECTURE_REIMPLANTATION_H__

#include "elf_relocation.h"
#include "elf_header.h"
#include "section_header.h"

Elf32_Rel *lire_rel(char *nom_fichier, Elf32_Ehdr ehdr, Elf32_Shdr *shdr);

Elf32_Rela *lire_rela(char *nom_fichier, Elf32_Ehdr ehdr, Elf32_Shdr *shdr);

#endif
