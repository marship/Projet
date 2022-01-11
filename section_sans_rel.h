#ifndef __SECTION_SANS_REL_H__
#define __SECTION_SANS_REL_H__

#include "elf_relocation.h"
#include "elf_header.h"
#include "section_header.h"
#include "elf_types.h"
#include "elf_symbol.h"

uint32_t nb_rel(Elf32_Ehdr ehdr, Elf32_Shdr *shdr);
Elf32_Shdr *maj_section(Elf32_Ehdr ehdr, Elf32_Shdr *shdr);
uint32_t index_string(Elf32_Ehdr ehdr, Elf32_Shdr *shdr);
Elf32_Sym *maj_ndx(Elf32_Sym *sym, Elf32_Shdr *shdr, Elf32_Ehdr ehdr);

#endif
