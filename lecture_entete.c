#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lecture_entete.h"
#include "elf_header.h"


int read_uint16(uint16_t *ptr, FILE *stream, unsigned char endian) {
    if (endian < ELFDATA2LSB || endian > ELFDATA2MSB) {
        return fread(ptr, sizeof(uint16_t), 1, stream);
    }

    unsigned char b1, b2;

    if (fread(&b1, sizeof(char), 1, stream) == 0 || fread(&b2, sizeof(char), 1, stream) == 0) {
        return 0;
    }

    if (endian == ELFDATA2LSB) {
        *ptr = ((uint16_t) b2) << 8 | (uint16_t) b1;
    }
    else {
        *ptr = ((uint16_t) b1) << 8 | (uint16_t) b2;
    }
    return 1;
}

int read_uint32(uint32_t *ptr, FILE *stream, unsigned char endian) {
    if (endian < ELFDATA2LSB || endian > ELFDATA2MSB) {
        return fread(ptr, sizeof(uint32_t), 1, stream);
    }

    unsigned char b1, b2, b3, b4;

    if (fread(&b1, sizeof(char), 1, stream) == 0 || fread(&b2, sizeof(char), 1, stream) == 0
    || fread(&b3, sizeof(char), 1, stream) == 0 || fread(&b4, sizeof(char), 1, stream) == 0) {
        return 0;
    }

    if (endian == ELFDATA2LSB) {
        *ptr = ((uint32_t) b4) << 24 | ((uint32_t) b3) << 16 | ((uint32_t) b2) << 8 | (uint32_t) b1;
    }
    else {
        *ptr = ((uint32_t) b1) << 24 | ((uint32_t) b2) << 16 | ((uint32_t) b3) << 8 | (uint32_t) b4;
    }
    return 1;
}

void lire_entete(char *nom_fichier) {
    FILE *f;
    Elf32_Ehdr hdr;

    // Ouverture du fichier
    f = fopen(nom_fichier, "rb");

    if (f == NULL) {
        fprintf(stderr, "Error: Can't open file %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    // Lecture de e_ident
    for (int i = 0; i < EI_NIDENT; i++) {
        if (fread(&hdr.e_ident[i], sizeof(char), 1, f) == 0) {
            fprintf(stderr, "Error: %s: Failed to read file header\n", nom_fichier);
            exit(EXIT_FAILURE);
        }
    }

    // Vérification qu'il s'agit bien d'un fichier ELF
    if (hdr.e_ident[EI_MAG0] != ELFMAG0 || hdr.e_ident[EI_MAG1] != ELFMAG1
    || hdr.e_ident[EI_MAG2] != ELFMAG2 || hdr.e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "Error: Not an ELF file - it has the wrong magic bytes at the start\n");
        exit(EXIT_FAILURE);
    }

    // Lecture de la suite de l'entête
    if (read_uint16(&hdr.e_type, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint16(&hdr.e_machine, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint32(&hdr.e_version, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint32(&hdr.e_entry, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint32(&hdr.e_phoff, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint32(&hdr.e_shoff, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint32(&hdr.e_flags, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint16(&hdr.e_ehsize, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint16(&hdr.e_phentsize, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint16(&hdr.e_phnum, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint16(&hdr.e_shentsize, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint16(&hdr.e_shnum, f, hdr.e_ident[EI_DATA]) == 0
    || read_uint16(&hdr.e_shstrndx, f, hdr.e_ident[EI_DATA]) == 0) {
        fprintf(stderr, "Error: %s: Failed to read file header\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    // Fermeture du fichier
    fclose(f);

    // Affichage de l'entête
    printf("ELF Header:\n");

    printf("  Magic:   %02x %02x %02x %02x %02x %02x %02x ",
        hdr.e_ident[EI_MAG0], hdr.e_ident[EI_MAG1], hdr.e_ident[EI_MAG2], hdr.e_ident[EI_MAG3],
        hdr.e_ident[EI_CLASS], hdr.e_ident[EI_DATA], hdr.e_ident[EI_VERSION]
    );
    for (int i = EI_PAD; i < EI_NIDENT; i++) {
        printf("%02x ", hdr.e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             ");
    switch (hdr.e_ident[EI_CLASS]) {
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
        printf("<unknown: %x>\n", hdr.e_ident[EI_CLASS]);
    }

    printf("  Data:                              ");
    switch (hdr.e_ident[EI_DATA]) {
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
        printf("<unknown: %x>\n", hdr.e_ident[EI_DATA]);
    }

    printf("  Version:                           ");
    switch (hdr.e_ident[EI_VERSION]) {
    case EV_NONE:
        printf("0\n");
        break;
    case EV_CURRENT:
        printf("1 (current)\n");
        break;
    default:
        printf("%d <unknown: %%lx>\n", hdr.e_ident[EI_VERSION]);
    }

    printf("  OS/ABI:                            UNIX - System V\n");
    printf("  ABI Version:                       0\n");

    printf("  Type:                              ");
    switch (hdr.e_type) {
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
        printf("<unknown>: %x\n", hdr.e_type);
    }

    printf("  Machine:                           ");
    switch (hdr.e_machine) {
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
        printf("<unknown>: 0x%x\n", hdr.e_machine);
    }

    printf("  Version:                           0x%x\n", hdr.e_version);
    printf("  Entry point address:               0x%x\n", hdr.e_entry);
    printf("  Start of program headers:          %u (bytes into file)\n", hdr.e_phoff);
    printf("  Start of section headers:          %u (bytes into file)\n", hdr.e_shoff);
    printf("  Flags:                             0x%x\n", hdr.e_flags);
    printf("  Size of this header:               %u (bytes)\n", hdr.e_ehsize);
    printf("  Size of program headers:           %u (bytes)\n", hdr.e_phentsize);
    printf("  Number of program headers:         %u\n", hdr.e_phnum);
    printf("  Size of section headers:           %u (bytes)\n", hdr.e_shentsize);
    printf("  Number of section headers:         %u\n", hdr.e_shnum);
    printf("  Section header string table index: %u\n", hdr.e_shstrndx);
}
