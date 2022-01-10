#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "section_header.h"
#include "string_table.h"
#include "fonctions_utilitaires.h"


Elf32_Shdr *lire_section_header(FILE *f, Elf32_Ehdr ehdr, long taille)
{
    // Allocation de la place nécessaire
    Elf32_Shdr *shdr = malloc(ehdr.e_shnum * sizeof(Elf32_Shdr));
    
    if (shdr == NULL) {
        fprintf(stderr, "ERREUR: Impossible d'allouer de la mémoire pour les en-têtes de section\n");
        exit(EXIT_FAILURE);
    }

    // Déplacement au début des en-têtes de section
    if (fseek(f, ehdr.e_shoff, SEEK_SET) != 0 || taille < ehdr.e_shoff + ehdr.e_shnum * ehdr.e_shentsize)
    {
        fprintf(stderr,
                "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour En-têtes de section\n",
                ehdr.e_shnum * ehdr.e_shentsize);
        exit(EXIT_FAILURE);
    }

    // Lecture des valeurs souhaitées
    for (int i = 0; i < ehdr.e_shnum; i++)
    {
        if (read_uint32(&shdr[i].sh_name, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_type, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_flags, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_addr, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_offset, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_size, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_link, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_info, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_addralign, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&shdr[i].sh_entsize, f, ehdr.e_ident[EI_DATA]) == 0)
        {
            fprintf(stderr,
                    "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour En-têtes de section\n",
                    ehdr.e_shnum * ehdr.e_shentsize);
            exit(EXIT_FAILURE);
        }
    }

    return shdr;
}


void afficher_section_header(Elf32_Shdr *shdr, Elf32_Ehdr ehdr, char *shstrtab)
{
    printf("\nSection Headers:\n");
    printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

    for (int i = 0; i < ehdr.e_shnum; i++)
    {
        if (i < 10) {
            printf("  [ %d] ", i);
        } else {
            printf("  [%d] ", i);
        }

        int len = afficher_chaine(shstrtab, shdr[i].sh_name, 17);
        for (int j = len; j < 18; j++) {
            printf(" ");
        }

        switch (shdr[i].sh_type)
        {
        case SHT_NULL:
            printf("NULL            ");
            break;
        
        case SHT_PROGBITS:
            printf("PROGBITS        ");
            break;
        
        case SHT_SYMTAB:
            printf("SYMTAB          ");
            break;
        
        case SHT_STRTAB:
            printf("STRTAB          ");
            break;

        case SHT_RELA:
            printf("RELA            ");
            break;
        
        case SHT_HASH:
            printf("HASH            ");
            break;
        
        case SHT_DYNAMIC:
            printf("DYNAMIC         ");
            break;
        
        case SHT_NOTE:
            printf("NOTE            ");
            break;
        
        case SHT_NOBITS:
            printf("NOBITS          ");
            break;
        
        case SHT_REL:
            printf("REL             ");
            break;
        
        case SHT_SHLIB:
            printf("SHLIB           ");
            break;
        
        case SHT_DYNSYM:
            printf("DYNSYM          ");
            break;

        case SHT_LOPROC:
            printf("LOPROC+0        ");
            break;
        
        case SHT_HIPROC:
            printf("FILTER          ");
            break;
        
        case SHT_LOUSER:
            printf("LOUSER+0        ");
            break;

        case SHT_ARM_EXIDX:
            printf("ARM_EXIDX       ");
            break;

        case SHT_ARM_PREEMPTMAP:
            printf("ARM_PREEMPTMAP  ");
            break;

        case SHT_ARM_ATTRIBUTES:
            printf("ARM_ATTRIBUTES  ");
            break;

        case SHT_ARM_DEBUGOVERLAY:
            printf("ARM_DEBUGOVERLA ");
            break;

        case SHT_ARM_OVERLAYSECTION:
            printf("ARM_OVERLAYSECT ");
            break;

        default:
            if (shdr[i].sh_type > SHT_LOPROC && shdr[i].sh_type < SHT_HIPROC) {
                Elf32_Word x = shdr[i].sh_type - SHT_LOPROC;

                if (x <= 0xf) {
                    printf("LOPROC+0x%x      ", x);
                } else if (x <= 0xff) {
                    printf("LOPROC+0x%x     ", x);
                } else if (x <= 0xfff) {
                    printf("LOPROC+0x%x    ", x);
                } else if (x <= 0xffff) {
                    printf("LOPROC+0x%x   ", x);
                } else if (x <= 0xfffff) {
                    printf("LOPROC+0x%x  ", x);
                } else if (x <= 0xffffff) {
                    printf("LOPROC+0x%x ", x);
                } else if (x <= 0xfffffff) {
                    printf("LOPROC+0x%x ", x >> 4);
                } else {
                    printf("LOPROC+0x%x ", x >> 8);
                }
            }
            else if (shdr[i].sh_type > SHT_LOUSER && shdr[i].sh_type <= SHT_HIUSER) {
                Elf32_Word x = shdr[i].sh_type - SHT_LOUSER;

                if (x <= 0xf) {
                    printf("LOUSER+0x%x      ", x);
                } else if (x <= 0xff) {
                    printf("LOUSER+0x%x     ", x);
                } else if (x <= 0xfff) {
                    printf("LOUSER+0x%x    ", x);
                } else if (x <= 0xffff) {
                    printf("LOUSER+0x%x   ", x);
                } else if (x <= 0xfffff) {
                    printf("LOUSER+0x%x  ", x);
                } else if (x <= 0xffffff) {
                    printf("LOUSER+0x%x ", x);
                } else if (x <= 0xfffffff) {
                    printf("LOUSER+0x%x ", x >> 4);
                } else {
                    printf("LOUSER+0x%x ", x >> 8);
                }
            }
            else {
                printf("%08x: <unkn ", shdr[i].sh_type);
            }
            break;
        }

        printf("%.8x ", shdr[i].sh_addr);
        printf("%.6x ", shdr[i].sh_offset);
        printf("%.6x ", shdr[i].sh_size);
        printf("%.2x ", shdr[i].sh_entsize);

        int nb_flags = compter_flags(shdr[i].sh_flags);

        if ((shdr[i].sh_flags & SHF_MASKOS) != 0) {
            nb_flags -= 7;
        }
        if ((shdr[i].sh_flags & SHF_MASKPROC) != 0) {
            nb_flags -= 3;
        }

        if (nb_flags == 2) {
            printf(" ");
        } else if (nb_flags == 1) {
            printf("  ");
        } else if (nb_flags == 0) {
            printf("   ");
        }

        if ((shdr[i].sh_flags & SHF_WRITE) != 0) {
            printf("W");
        }
        if ((shdr[i].sh_flags & SHF_ALLOC) != 0) {
            printf("A");
        }
        if ((shdr[i].sh_flags & SHF_EXECINSTR) != 0) {
            printf("X");
        }
        if ((shdr[i].sh_flags & SHF_MERGE) != 0) {
            printf("M");
        }
        if ((shdr[i].sh_flags & SHF_STRINGS) != 0) {
            printf("S");
        }
        if ((shdr[i].sh_flags & SHF_INFO_LINK) != 0) {
            printf("I");
        }
        if ((shdr[i].sh_flags & SHF_LINK_ORDER) != 0) {
            printf("L");
        }
        if ((shdr[i].sh_flags & SHF_OS_NONCONFORMING) != 0) {
            printf("O");
        }
        if ((shdr[i].sh_flags & SHF_GROUP) != 0) {
            printf("G");
        }
        if ((shdr[i].sh_flags & SHF_TLS) != 0) {
            printf("T");
        }
        if ((shdr[i].sh_flags & SHF_COMPRESSED) != 0) {
            printf("C");
        }
        if ((shdr[i].sh_flags & SHF_MASKOS) != 0) {
            printf("o");
        }
        if ((shdr[i].sh_flags & SHF_MASKPROC) != 0) {
            printf("p");
        }

        if (shdr[i].sh_link >= 100) {
            printf("%d ", shdr[i].sh_link);
        } else if (shdr[i].sh_link >= 10) {
            printf(" %d ", shdr[i].sh_link);
        } else {
            printf("  %d ", shdr[i].sh_link);
        }
        
        if (shdr[i].sh_info >= 100) {
            printf("%d ", shdr[i].sh_info);
        } else if (shdr[i].sh_info >= 10) {
            printf(" %d ", shdr[i].sh_info);
        } else {
            printf("  %d ", shdr[i].sh_info);
        }

        if (shdr[i].sh_addralign >= 10) {
            printf("%d", shdr[i].sh_addralign);
        } else {
            printf(" %d", shdr[i].sh_addralign);
        }
        printf("\n");
    }
    printf("Key to Flags:\n");
    printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
    printf("  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
    printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
    printf("  y (purecode), p (processor specific)\n");
}
