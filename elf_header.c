#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "elf_header.h"
#include "fonctions_utilitaires.h"


Elf32_Ehdr lire_elf_header(FILE *f, char *nom_fichier)
{
    Elf32_Ehdr ehdr;

    // Lecture de e_ident
    for (int i = 0; i < EI_NIDENT; i++)
    {
        if (fread(&ehdr.e_ident[i], sizeof(char), 1, f) == 0)
        {
            fprintf(stderr, "ERREUR: %s: Échec de lecture de l'en-tête du fichier\n", nom_fichier);
            exit(EXIT_FAILURE);
        }
    }

    // Vérification qu'il s'agit bien d'un fichier ELF
    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 || ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 || ehdr.e_ident[EI_MAG3] != ELFMAG3)
    {
        fprintf(stderr, "ERREUR: N'est pas un fichier ELF – a les mauvais octets magiques au départ\n");
        exit(EXIT_FAILURE);
    }
 
    // Lecture de la suite de l'entête
    if (read_uint16(&ehdr.e_type, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint16(&ehdr.e_machine, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint32(&ehdr.e_version, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint32(&ehdr.e_entry, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint32(&ehdr.e_phoff, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint32(&ehdr.e_shoff, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint32(&ehdr.e_flags, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint16(&ehdr.e_ehsize, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint16(&ehdr.e_phentsize, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint16(&ehdr.e_phnum, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint16(&ehdr.e_shentsize, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint16(&ehdr.e_shnum, f, ehdr.e_ident[EI_DATA]) == 0 ||
        read_uint16(&ehdr.e_shstrndx, f, ehdr.e_ident[EI_DATA]) == 0)
    {
        fprintf(stderr, "ERREUR: %s: Échec de lecture de l'en-tête du fichier\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    return ehdr;
}


void afficher_elf_header(Elf32_Ehdr ehdr)
{
    // Affichage de l'entête
    printf("ELF Header:\n");

    // Magic
    printf("  Magic:   %02x %02x %02x %02x %02x %02x %02x ",
           ehdr.e_ident[EI_MAG0], ehdr.e_ident[EI_MAG1], ehdr.e_ident[EI_MAG2], ehdr.e_ident[EI_MAG3],
           ehdr.e_ident[EI_CLASS], ehdr.e_ident[EI_DATA], ehdr.e_ident[EI_VERSION]);
    for (int i = EI_PAD; i < EI_NIDENT; i++)
    {
        printf("%02x ", ehdr.e_ident[i]);
    }
    printf("\n");

    // Class
    printf("  Class:                             ");
    switch (ehdr.e_ident[EI_CLASS])
    {
    case ELFCLASSNONE:
        printf("none\n");
        break;
    case ELFCLASS32:
        printf("ELF32\n");
        break;
    case ELFCLASS64:
        printf("ELF64\n");
        break;
    default:
        printf("<unknown: %x>\n", ehdr.e_ident[EI_CLASS]);
    }

    // Data
    printf("  Data:                              ");
    switch (ehdr.e_ident[EI_DATA])
    {
    case ELFDATANONE:
        printf("none\n");
        break;
    case ELFDATA2LSB:
        printf("2's complement, little endian\n");
        break;
    case ELFDATA2MSB:
        printf("2's complement, big endian\n");
        break;
    default:
        printf("<unknown: %x>\n", ehdr.e_ident[EI_DATA]);
    }

    // Version
    printf("  Version:                           ");
    switch (ehdr.e_ident[EI_VERSION])
    {
    case EV_NONE:
        printf("0\n");
        break;
    case EV_CURRENT:
        printf("1 (current)\n");
        break;
    default:
        printf("%d <unknown: %%lx>\n", ehdr.e_ident[EI_VERSION]);
    }

    // OS/ABI
    printf("  OS/ABI:                            UNIX - System V\n");

    // ABI Version
    printf("  ABI Version:                       0\n");

    // Type
    printf("  Type:                              ");
    switch (ehdr.e_type)
    {
    case ET_NONE:
        printf("NONE (None)\n");
        break;
    case ET_REL:
        printf("REL (Relocatable file)\n");
        break;
    case ET_EXEC:
        printf("EXEC (Executable file)\n");
        break;
    case ET_DYN:
        printf("DYN (Shared object file)\n");
        break;
    case ET_CORE:
        printf("CORE (Core file)\n");
        break;
    case ET_LOPROC:
        printf("Processor Specific: (ff00)\n");
        break;
    case ET_HIPROC:
        printf("Processor Specific: (ffff)\n");
        break;
    default:
        printf("<unknown>: %x\n", ehdr.e_type);
    }

    // Machine
    printf("  Machine:                           ");
    switch (ehdr.e_machine)
    {
    case EM_NONE:
        printf("None\n");
        break;
    case EM_M32:
        printf("WE32100\n");
        break;
    case EM_SPARC:
        printf("Sparc\n");
        break;
    case EM_386:
        printf("Intel 80386\n");
        break;
    case EM_68K:
        printf("MC68000\n");
        break;
    case EM_88K:
        printf("MC88000\n");
        break;
    case EM_860:
        printf("Intel 80860\n");
        break;
    case EM_MIPS:
        printf("MIPS R3000\n");
        break;
    case EM_MIPS_RS4_BE:
        printf("MIPS R4000 big-endian\n");
        break;
    case EM_ARM:
        printf("ARM\n");
        break;
    default:
        printf("<unknown>: 0x%x\n", ehdr.e_machine);
    }

    // Version
    printf("  Version:                           0x%x\n", ehdr.e_version);

    // Entry point address
    printf("  Entry point address:               0x%x\n", ehdr.e_entry);

    // Start of program headers
    printf("  Start of program headers:          %u (bytes into file)\n", ehdr.e_phoff);

    // Start of section headers
    printf("  Start of section headers:          %u (bytes into file)\n", ehdr.e_shoff);

    // Flags
    printf("  Flags:                             0x%x", ehdr.e_flags);

    if (ehdr.e_machine == EM_ARM) {
        // relocatable executable
        if ((ehdr.e_flags & EF_ARM_REL_EXEC) != 0) {
            printf(", relocatable executable");
        }

        // position independent
        if ((ehdr.e_flags & EF_ARM_POS_IND) != 0) {
            printf(", position independent");
        }

        // EABI        
        int abi = (ehdr.e_flags & EF_ARM_ABIMASK) >> 24;

        if (abi > 5) {
            printf(", Version%d EABI", abi);
        }
        else if (abi > 0) {
            printf(", <unrecognized EABI>");
        }

        // soft-float ABI
        if ((ehdr.e_flags & EF_ARM_ABI_FLOAT_SOFT) != 0) {
            printf(", soft-float ABI");
        }

        // hard-float ABI
        if ((ehdr.e_flags & EF_ARM_ABI_FLOAT_HARD) != 0) {
            printf(", hard-float ABI");
        }

        // LE8
        if ((ehdr.e_flags & EF_ARM_LE8) != 0) {
            printf(", LE8");
        }

        // BE8
        if ((ehdr.e_flags & EF_ARM_BE8) != 0) {
            printf(", BE8");
        }

        // Unknown
        if ((ehdr.e_flags & EF_ARM_UNKNOWN) != 0) {
            printf(", <unknown>");
        }
    }
    printf("\n");

    // Size of this header
    printf("  Size of this header:               %u (bytes)\n", ehdr.e_ehsize);

    // Size of program headers
    printf("  Size of program headers:           %u (bytes)\n", ehdr.e_phentsize);

    // Number of program headers
    printf("  Number of program headers:         %u\n", ehdr.e_phnum);

    // Size of section headers
    printf("  Size of section headers:           %u (bytes)\n", ehdr.e_shentsize);

    // Number of section headers
    printf("  Number of section headers:         %u\n", ehdr.e_shnum);

    // Section header string table index
    printf("  Section header string table index: %u\n", ehdr.e_shstrndx);
}
