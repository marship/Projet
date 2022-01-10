#ifndef __SECTION_H__
#define __SECTION_H__

#include "section_header.h"


unsigned char *lire_section(FILE *f, Elf32_Shdr *shdr, int num);

void afficher_section(unsigned char *sect, char *shstrtab, Elf32_Shdr shdr);


#endif
