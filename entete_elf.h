#ifndef __ENTETE_ELF_H__
#define __ENTETE_ELF_H__

#include <stdint.h>

/* TOUT CECI SORT DE LA DOCUMENTATION DE L'ENTETE D'UN FICHIER ELF */

// Redéfinie plus bas
#define EI_NIDENT 16

/* ELF32 types de base en 32-bit */
typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef uint32_t Elf32_Word;

/* ELF32 Structure de l'entête en 32-bit */
typedef struct
{
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Ehdr;

/* e_ident */
// e_ident est un tableau dont chaque indice possède une certaine valeur
#define EI_MAG0 0 /* File identification */                  // Contient la valeur : 0x7f
#define EI_MAG1 1 /* File identification */                  // Contient la valeur : 'E'
#define EI_MAG2 2 /* File identification */                  // Contient la valeur : 'L'
#define EI_MAG3 3 /* File identification */                  // Contient la valeur : 'F'
#define EI_CLASS 4 /* File class */                          // Contient la valeur : 0 Si correspond à ELFCLASSNONE donc Invalid class
                                                             // Contient la valeur : 1 Si correspond à ELFCLASS32 donc 32-bit objects
                                                             // Contient la valeur : 2 Si correspond à ELFCLASS64 donc 64-bit objects
#define EI_DATA 5 /* Data encoding */                        // Contient la valeur : 0 Si correspond à ELFDATANONE donc Invalid data encoding
                                                             // Contient la valeur : 1 Si correspond à ELFDATA2LSB donc byte address zero on the left : 04 03 02 01 = 0x 01020304 Little Endian
                                                             // Contient la valeur : 2 Si correspond à ELFDATA2MSB donc byte address zero on the left : 01 02 03 04 = 0x 01020304 Big Endian
#define EI_VERSION 6 /* File version */                      // Souvent égale à EV_CURRENT

#define EI_PAD 7 /* Start of padding bytes */                // Début des octets inutilisés dans e_ident (Ces octets sont réservés et mis à zéro, Les programmes qui lisent les fichiers d'objets doivent les ignorer)
                                                             // Cet valeur changera dans le futur si l'on donne une signification aux octets actuellement inutilisés
#define EI_NIDENT 16 /* Size of e_ident[] */                 // Contient la taille du tableau e_ident[]

/* EI_MAG0 à EI_MAG3 */
#define ELFMAG0 0x7f     /* e_ident[EI_MAG0] */
#define ELFMAG1 'E'      /* e_ident[EI_MAG1] */
#define ELFMAG2 'L'      /* e_ident[EI_MAG2] */
#define ELFMAG3 'F'      /* e_ident[EI_MAG3] */
#define ELFMAG "\177ELF" /* Identifiant ELF complet en string */
#define SELFMAG 4        /* Taille de l'identifiant ELF */

/* EI_CLASS */
#define ELFCLASSNONE 0 /* Invalid class */
#define ELFCLASS32 1   /* 32-bit objects */
#define ELFCLASS64 2   /* 64-bit objects */

/* EI_DATA */
#define ELFDATANONE 0 /* Invalid data encoding */
#define ELFDATA2LSB 1 /* Little Endian */
#define ELFDATA2MSB 2 /* Big Endian */

/* e_type */
#define ET_NONE 0        /* No file type */
#define ET_REL 1         /* Relocatable file */
#define ET_EXEC 2        /* Executable file */
#define ET_DYN 3         /* Shared object file */
#define ET_CORE 4        /* Core file */
#define ET_LOPROC 0xff00 /* Processor-specific */
#define ET_HIPROC 0xffff /* Processor-specific */

/* e_machine */
#define EM_NONE 0         /* No machine */
#define EM_M32 1          /* AT&T WE 32100 */
#define EM_SPARC 2        /* SPARC */
#define EM_386 3          /* Intel Architecture */
#define EM_68K 4          /* Motorola 68000 */
#define EM_88K 5          /* Motorola 88000 */
#define EM_860 7          /* Intel 80860 */
#define EM_MIPS 8         /* MIPS RS3000 Big-Endian */
#define EM_MIPS_RS4_BE 10 /* MIPS RS4000 Big-Endian */
//      RESERVED        11-16   Reserved for future use
//      RESERVED        17-39   Other Machine
#define EM_ARM 40 /* ARM 32-bit architecture (AARCH32) */
//      RESERVED        41-243  Other Machine

/* e_version */
#define EV_NONE 0    /* Invalid version */
#define EV_CURRENT 1 /* Current version */

/* Special Section Indexes */
#define SHN_UNDEF 0          /* This value marks an undefined,missing,irrelevant or otherwise meaningless section reference. */
#define SHN_LORESERVE 0xff00 /* This value specifies the lower bound of the range of reserved indexes. */
#define SHN_LOPROC 0xff00    // Values in this inclusive range are reserved
#define SHN_HIPROC 0xff1f    // for processor-specific semantics.
#define SHN_ABS 0xfff1       /* This value specifies absolute values for the corresponding reference. */
#define SHN_COMMON 0xfff2    /* Symbols defined relative to this section are common symbols. */
#define SHN_HIRESERVE 0xffff /* This value specifies the upper bound of the range of reserved indexes. */

/* ELF32 section header. */
typedef struct
{
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_Shdr;


#endif
