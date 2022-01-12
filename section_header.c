#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "section_header.h"
#include "string_table.h"
#include "fonctions_utilitaires.h"

#define REL_ENTSIZE    8
#define RELA_ENTSIZE   12
#define SYMTAB_ENTSIZE 16
#define DYNSYM_ENTSIZE 16
#define GROUP_ENTSIZE  4


Elf32_Shdr *lire_section_header(FILE *f, Elf32_Ehdr ehdr, long taille)
{
    if (ehdr.e_shnum == 0) {
        return NULL;
    }

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

        if (shdr[i].sh_link >= ehdr.e_shnum) {
            fprintf(stderr, "AVERTISSEMENT: La section %d a une valeur sh_link %d hors limite\n",
                    i, shdr[i].sh_link);
            exit(EXIT_FAILURE);
        }

        if ((shdr[i].sh_type == SHT_REL && shdr[i].sh_entsize != REL_ENTSIZE) ||
            (shdr[i].sh_type == SHT_RELA && shdr[i].sh_entsize != RELA_ENTSIZE) ||
            (shdr[i].sh_type == SHT_SYMTAB && shdr[i].sh_entsize != SYMTAB_ENTSIZE) ||
            (shdr[i].sh_type == SHT_DYNSYM && shdr[i].sh_entsize != DYNSYM_ENTSIZE) ||
            (shdr[i].sh_type == SHT_GROUP && shdr[i].sh_entsize != GROUP_ENTSIZE))
        {
            fprintf(stderr, "ERREUR: La section %d a une sh_entsize de %016x invalide\n",
                    i, shdr[i].sh_entsize);
            exit(EXIT_FAILURE);
        }
    }

    return shdr;
}


void afficher_section_header(Elf32_Shdr *shdr, Elf32_Ehdr ehdr, char *shstrtab)
{
    if (shdr == NULL) {
        printf("\nThere are no sections in this file.\n");
        return;
    }

    printf("\nSection Headers:\n");
    printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

    for (int i = 0; i < ehdr.e_shnum; i++)
    {
        Elf32_Word type = shdr[i].sh_type;
        Elf32_Word link = shdr[i].sh_link;
        Elf32_Word info = shdr[i].sh_info;

        if ((type == SHT_REL || type == SHT_RELA || type == SHT_GROUP || type == SHT_SYMTAB_SHNDX || type == SHT_HASH)
            && shdr[link].sh_type != SHT_SYMTAB)
        {
            fprintf(stderr,
                    "AVERTISSEMENT: [%2d]: Le champ de liaison (%d) devrait indexer une section symtab.\n",
                    i, link);
            exit(EXIT_FAILURE);
        }

        if ((type == SHT_DYNAMIC || type == SHT_SYMTAB || type == SHT_DYNSYM) && shdr[link].sh_type != SHT_STRTAB) {
            fprintf(stderr,
                    "AVERTISSEMENT: [%2d]: Le champ de liaison (%d) devrait indexer une section de chaînes.\n",
                    i, link);
            exit(EXIT_FAILURE);
        }

        if ((type == SHT_REL || type == SHT_RELA) && shdr[info].sh_type != SHT_PROGBITS) {
            fprintf(stderr,
                    "AVERTISSEMENT: [%2d]: Le champ d'info (%d) devrait indexer une section réadressable.\n",
                    i, info);
            exit(EXIT_FAILURE);
        }

        if ((type == SHT_DYNAMIC || type == SHT_HASH || type == SHT_SYMTAB_SHNDX) && info != 0) {
            fprintf(stderr,
                    "AVERTISSEMENT: [%2d]: Valeur (%d) inattendue dans le champ d'info.\n",
                    i, info);
            exit(EXIT_FAILURE);
        }

        printf("  [%2d] ", i);

        int len = afficher_chaine(shstrtab, shdr[i].sh_name, 17);
        for (int j = len; j < 18; j++) {
            printf(" ");
        }

        switch (type)
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

        case SHT_INIT_ARRAY:
            printf("INIT_ARRAY      ");
            break;

        case SHT_FINI_ARRAY:
            printf("FINI_ARRAY      ");
            break;

        case SHT_PREINIT_ARRAY:
            printf("PREINIT_ARRAY   ");
            break;

        case SHT_GROUP:
            printf("GROUP           ");
            break;

        case SHT_SYMTAB_SHNDX:
            printf("SYMTAB SECTION  ");
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

        default:
            if (type == SHT_ARM_EXIDX && ehdr.e_machine == EM_ARM) {
                printf("ARM_EXIDX       ");
            }
            else if (type == SHT_ARM_PREEMPTMAP && ehdr.e_machine == EM_ARM) {
                printf("ARM_PREEMPTMAP  ");
            }
            else if (type == SHT_ARM_ATTRIBUTES && ehdr.e_machine == EM_ARM) {
                printf("ARM_ATTRIBUTES  ");
            }
            else if (type == SHT_ARM_DEBUGOVERLAY && ehdr.e_machine == EM_ARM) {
                printf("ARM_DEBUGOVERLA ");
            }
            else if (type == SHT_ARM_OVERLAYSECTION && ehdr.e_machine == EM_ARM) {
                printf("ARM_OVERLAYSECT ");
            }
            else if (type > SHT_LOPROC && type < SHT_HIPROC) {
                Elf32_Word x = type - SHT_LOPROC;

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
            else if (type > SHT_LOUSER && type <= SHT_HIUSER) {
                Elf32_Word x = type - SHT_LOUSER;

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
                printf("%08x: <unkn ", type);
            }
            break;
        }

        printf("%.8x ", shdr[i].sh_addr);
        printf("%.6x ", shdr[i].sh_offset);
        printf("%.6x ", shdr[i].sh_size);
        printf("%.2x ", shdr[i].sh_entsize);

        int nb_flags = compter_bits(shdr[i].sh_flags);

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

        printf("%3d ", link);        
        printf("%3d ", info);
        printf("%2d", shdr[i].sh_addralign);

        printf("\n");
    }
    printf("Key to Flags:\n");
    printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
    printf("  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
    printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n  ");
    if (ehdr.e_machine == EM_ARM) {
        printf("y (purecode), ");
    }
    printf("p (processor specific)\n");
}
