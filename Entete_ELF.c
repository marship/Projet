#include <stdio.h>
#include <stdlib.h>

/* TOUT CECI SORT DE LA DOCUMENTATION DE L'ENTETE D'UN FICHIER ELF */

// Redéfinie plus bas
#define EI_NIDENT       16

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
// e_ident est un tableau dont chaque indice possède une certaine valeure
#define EI_MAG0 	    0 	/* File identification */                   // Contient la valeur : 0x7f
#define EI_MAG1 	    1 	/* File identification */                   // Contient la valeur : 'E'
#define EI_MAG2 	    2 	/* File identification */                   // Contient la valeur : 'L'
#define EI_MAG3 	    3 	/* File identification */                   // Contient la valeur : 'F'
#define EI_CLASS 	    4 	/* File class */                            // Contient la valeur : 0 Si correspond à ELFCLASSNONE donc Invalid class
                                                                        // Contient la valeur : 1 Si correspond à ELFCLASS32 donc 32-bit objects
                                                                        // Contient la valeur : 2 Si correspond à ELFCLASS64 donc 64-bit objects           
#define EI_DATA 	    5 	/* Data encoding */                         // Contient la valeur : 0 Si correspond à ELFDATANONE donc Invalid data encoding
                                                                        // Contient la valeur : 1 Si correspond à ELFDATA2LSB donc byte address zero on the left : 04 03 02 01 = 0x 01020304
                                                                        // Contient la valeur : 2 Si correspond à ELFDATA2MSB donc byte address zero on the left : 01 02 03 04 = 0x 01020304
#define EI_VERSION 	    6 	/* File version */                          // Souvent égale à EV_CURRENT
#define EI_OSABI 	    7 	/* Operating system/ABI identification */   // Contient la valeur : 0  Si correspond à ELFOSABI_NONE donc No extensions or unspecified
                                                                        // Contient la valeur : 1  Si correspond à ELFOSABI_HPUX donc Hewlett-Packard HP-UX
                                                                        // Contient la valeur : 2  Si correspond à ELFOSABI_NETBSD donc NetBSD
                                                                        // Contient la valeur : 3  Si correspond à ELFOSABI_GNU donc GNU
                                                                        // Contient la valeur : 3  Si correspond à ELFOSABI_LINUX  donc Linux 
                                                                        // Contient la valeur : 6  Si correspond à ELFOSABI_SOLARIS donc Sun Solaris
                                                                        // Contient la valeur : 7  Si correspond à ELFOSABI_AIX donc AIX
                                                                        // Contient la valeur : 8  Si correspond à ELFOSABI_IRIX donc IRIX
                                                                        // Contient la valeur : 9  Si correspond à ELFOSABI_FREEBSD donc FreeBSD
                                                                        // Contient la valeur : 10 Si correspond à ELFOSABI_TRU64 donc Compaq TRU64 UNIX
                                                                        // Contient la valeur : 11 Si correspond à ELFOSABI_MODESTO donc Novell Modesto
                                                                        // Contient la valeur : 12 Si correspond à ELFOSABI_OPENBSD donc Open BSD
                                                                        // Contient la valeur : 13 Si correspond à ELFOSABI_OPENVMS donc Open VMS
                                                                        // Contient la valeur : 14 Si correspond à ELFOSABI_NSK  donc Hewlett-Packard Non-Stop Kernel
                                                                        // Contient la valeur : 15 Si correspond à ELFOSABI_AROS donc Amiga Research OS
                                                                        // Contient la valeur : 16 Si correspond à ELFOSABI_FENIXOS donc The FenixOS highly scalable multi-core OS
                                                                        // Contient la valeur : 17 Si correspond à ELFOSABI_CLOUDABI donc Nuxi CloudABI
                                                                        // Contient la valeur : 18 Si correspond à ELFOSABI_OPENVOS donc Stratus Technologies OpenVOS
                                                                        // Contient la valeur : 64-255 Si correspond à Architecture-specific value range
#define EI_ABIVERSION 	8 	/* ABI version */                           // Dépend du champ EI_OSABI
                                                                        // Contient la valeur : 0 Si aucune valeur pour EI_OSABI par le supplément processeur ou si aucune valeur de version spécifiée pour une valeur particulière de l'octet EI_OSABI Donc indiqué comme non spécifié.
#define EI_PAD 	        9 	/* Start of padding bytes */                // Début des octets inutilisés dans e_ident (Ces octets sont réservés et mis à zéro, Les programmes qui lisent les fichiers d'objets doivent les ignorer)
                                                                        // Cet valeur changera dans le futur si l'on donne une signification aux octets actuellement inutilisés
#define EI_NIDENT       16  /* Size of e_ident[] */                     // Contient la taille du tableau e_ident[]

/* EI_MAG0 à EI_MAG3 */
#define ELFMAG0         0x7f        /* e_ident[EI_MAG0] */
#define ELFMAG1         'E'	        /* e_ident[EI_MAG1] */
#define ELFMAG2         'L'	        /* e_ident[EI_MAG2] */
#define ELFMAG3         'F'	        /* e_ident[EI_MAG3] */
#define ELFMAG          "\177ELF"   /* Identifiant ELF complet en string */
#define SELFMAG         4           /* Taille de l'identifiant ELF */

/* e_type */
#define ET_NONE     0       /* No file type */
#define ET_REL      1       /* Relocatable file */
#define ET_EXEC     2       /* Executable file */
#define ET_DYN      3       /* Shared object file */
#define ET_CORE     4       /* Core file */
#define ET_LOOS 	0xfe00 	/* Operating system-specific */
#define ET_HIOS 	0xfeff 	/* Operating system-specific */
#define ET_LOPROC   0xff00  /* Processor-specific */
#define ET_HIPROC   0xffff  /* Processor-specific */


/* e_machine */
#define EM_NONE         0       /* No machine */   
#define EM_M32          1       /* AT&T WE 32100 */
#define EM_SPARC        2       /* SPARC */
#define EM_386          3       /* Intel Architecture */
#define EM_68K          4       /* Motorola 68000 */
#define EM_88K          5       /* Motorola 88000 */
#define EM_IAMCU        6       /* Intel MCU */
#define EM_860          7       /* Intel 80860 */
#define EM_MIPS         8       /* MIPS RS3000 Big-Endian */
#define EM_S370         9       /* IBM System/370 Processor */
#define EM_MIPS_RS4_BE  10      /* MIPS RS4000 Big-Endian */
//      RESERVED        11-16   Reserved for future use
//      RESERVED        17-39   Other Machine
#define EM_ARM          40      /* ARM 32-bit architecture (AARCH32) */
//      RESERVED        41-243  Other Machine


/* e_version */
#define EV_NONE     0       /* Invalid versionn */
#define EV_CURRENT  1       /* Current version */


/* e_entry */


/* e_phoff */

/* e_shoff */

/* e_flags */

/* e_ehsize */

/* e_phentsize */

/* e_phnum */

/* e_shentsize */

/* e_shnum */

/* e_shstrndx */



int main(int argc, char const *argv[])
{
    return 0;
}
