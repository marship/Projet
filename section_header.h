#ifndef __SECTION_HEADER_H__
#define __SECTION_HEADER_H__

#include "elf_types.h"
#include "elf_header.h"


typedef struct
{
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_Shdr;

/* sh_type */
#define SHT_NULL               0
#define SHT_PROGBITS           1
#define SHT_SYMTAB             2
#define SHT_STRTAB             3
#define SHT_RELA               4
#define SHT_HASH               5
#define SHT_DYNAMIC            6
#define SHT_NOTE               7
#define SHT_NOBITS             8
#define SHT_REL                9
#define SHT_SHLIB              10
#define SHT_DYNSYM             11
#define SHT_INIT_ARRAY         14
#define SHT_FINI_ARRAY         15
#define SHT_PREINIT_ARRAY      16
#define SHT_GROUP              17
#define SHT_SYMTAB_SHNDX       18
#define SHT_LOPROC             0x70000000
#define SHT_HIPROC             0x7fffffff
#define SHT_LOUSER             0x80000000
#define SHT_HIUSER             0xffffffff
#define SHT_ARM_EXIDX          0x70000001
#define SHT_ARM_PREEMPTMAP     0x70000002
#define SHT_ARM_ATTRIBUTES     0x70000003
#define SHT_ARM_DEBUGOVERLAY   0x70000004
#define SHT_ARM_OVERLAYSECTION 0x70000005

/* sh_flags */
#define SHF_WRITE            0x1
#define SHF_ALLOC            0x2
#define SHF_EXECINSTR        0x4
#define SHF_MERGE            0x10
#define SHF_STRINGS          0x20
#define SHF_INFO_LINK        0x40
#define SHF_LINK_ORDER       0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP            0x200
#define SHF_TLS              0x400
#define SHF_COMPRESSED       0x800
#define SHF_MASKOS           0x0ff00000
#define SHF_MASKPROC         0xf0000000

/* Special Section Indexes */
#define SHN_UNDEF     0       /* This value marks an undefined,missing,irrelevant or otherwise meaningless section reference. */
#define SHN_LORESERVE 0xff00  /* This value specifies the lower bound of the range of reserved indexes. */
#define SHN_LOPROC    0xff00  // Values in this inclusive range are reserved
#define SHN_HIPROC    0xff1f  // for processor-specific semantics.
#define SHN_ABS       0xfff1  /* This value specifies absolute values for the corresponding reference. */
#define SHN_COMMON    0xfff2  /* Symbols defined relative to this section are common symbols. */
#define SHN_HIRESERVE 0xffff  /* This value specifies the upper bound of the range of reserved indexes. */


/**
 * @brief Fonction qui renvoie un tableau de structures Elf32_Shdr correspondant aux entêtes de sections du fichier ELF passé en paramètre
 *
 * @param f (FILE *) Fichier ELF ouvert
 * @param ehdr (Elf32_Ehdr *) Structure contenant l'entête ELF
 * @param taille (long) Taille (en octets) du fichier f
 * @return (Elf32_Shdr *) Renvoie un tableau de Elf32_Shdr correspondant aux entêtes des sections
 */
Elf32_Shdr *lire_section_header(FILE *f, Elf32_Ehdr ehdr, long taille);

/**
 * @brief Fonction qui affiche les entêtes des sections d'un fichier ELF
 *
 * @param shdr (Elf32_Shdr *) Tableau contenant les entêtes des sections
 * @param ehdr (Elf32_Ehdr) Structure contant l'entête du fichier ELF
 * @param shstrtab (char *) Tableau correspondant aux chaines de caractères des entêtes de sections
 */
void afficher_section_header(Elf32_Shdr *shdr, Elf32_Ehdr ehdr, char *shstrtab);


#endif
