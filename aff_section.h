#ifndef __AFF_SECTION_H__
#define __AFF_SECTION_H__

#include "elf_types.h"
#include "elf_header.h"
#include "section_header.h"

char *lire_une_section(char *nom_fichier, Elf32_Shdr *shdr, int num);

void aff_une_section(char *sect, uint32_t nbChar);

#endif
