#ifndef __LECTURE_SYMBOLE_H__
#define __LECTURE_SYMBOLE_H__

#include "elf_relocation.h"
#include "elf_header.h"
#include "section_header.h"
#include "elf_types.h"
#include "elf_symbol.h"

/**
 * @brief 
 * 
 * TO DO Description
 * 
 */
Elf32_Sym *lire_symbole(char *nom_fichier, Elf32_Ehdr ehdr, Elf32_Shdr *shdr);

/**
 * @brief 
 * 
 * TO DO Description
 * 
 */
void afficher_symboles(Elf32_Sym *sym, Elf32_Ehdr ehdr, Elf32_Shdr *shdr);

#endif
