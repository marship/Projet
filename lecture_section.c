#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lecture_section.h"
#include "fonctions_utilitaires.h"

// Numéro de Section
int noSection;

// Nombre de Symbole
int nbSym;

Elf32_Shdr *lire_symbole(char *nom_fichier, Elf32_Ehdr ehdr)
{
    FILE *f;

    // Allocation de la place nécessaire
    Elf32_Shdr *shdr = malloc(ehdr.e_shnum * sizeof(Elf32_Shdr));

    // Ouverture du fichier
    f = fopen(nom_fichier, "rb");

    if (f == NULL)
    {
        fprintf(stderr, "Impossible d'ouvir le fichier : %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    // Deplacement au début des symboles
    if (fseek(f, ehdr.e_shoff, SEEK_SET) != 0)
    {
        fprintf(stderr, "Erreur de lecture du fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    // Lecture des valeurs souhaitées
    for (int i = 0; i < ehdr.e_shnum; i++)
    {
        if (read_uint32(&shdr[i].sh_name, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_type, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_flags, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_addr, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_offset, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_size, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_link, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_info, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_addralign, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&shdr[i].sh_entsize, f, ehdr.e_ident[EI_DATA]) == 0)
        {
            fprintf(stderr, "Erreur: %s: Echec de la lecteur du symbole %d\n", nom_fichier, i);
            exit(EXIT_FAILURE);
        }
    }

    fclose(f);
    return shdr;
}

void afficher_section(Elf32_Shdr *shdr, Elf32_Ehdr ehdr){
    printf("Il y a %d en-tete de sections, qui commencent a offset 0x%d\n", ehdr.e_shnum, ehdr.e_shoff);
    printf("Section Headers :\n");
    printf("[Nb] Name\tType\tAddr\tOff\tSize\tES FLG LK INF AL\n");
    for (int i = 0; i < ehdr.e_shnum; i++){
        printf("[%d] ", i);
        // Name
        switch (shdr[i].sh_type)
        {
        case SHT_NULL:
            printf("NULL\t");
            break;
        
        case SHT_PROGBITS:
            printf("PROGBITS\t");
            break;
        
        case SHT_SYMTAB:
            printf("SYMTAB\t");
            break;
        
        case SHT_RELA:
            printf("RELA\t");
            break;
        
        case SHT_HASH:
            printf("HASH\t");
            break;
        
        case SHT_DYNAMIC:
            printf("DYNAMIC\t");
            break;
        
        case SHT_NOTE:
            printf("NOTE\t");
            break;
        
        case SHT_NOBITS:
            printf("NOBITS\t");
            break;
        
        case SHT_REL:
            printf("REL\t");
            break;
        
        case SHT_SHLIB:
            printf("SHLIB\t");
            break;
        
        case SHT_DYNSYM:
            printf("DYNSYM\t");
            break;
        
        case SHT_LOPROC:
            printf("LOPROC\t");
            break;
        
        case SHT_HIPROC:
            printf("HIPROC\t");
            break;
        
        case SHT_LOUSER:
            printf("LOUSER\t");
            break;
        
        case SHT_HIUSER:
            printf("HIUSER\t");
            break;
        
        default:
            printf("Inconnu\t");
            break;
        }
        printf("%8x", shdr[i].sh_addr);
        printf("%6x", shdr[i].sh_offset);
        printf("%6x", shdr[i].sh_size);
        printf("%2x", shdr[i].sh_entsize);

        switch (shdr[i].sh_flags)
        {
        case SHF_WRITE:
            printf("W ");
            break;

        case SHF_ALLOC:
            printf("A ");
            break;

        case SHF_EXECINSTR:
            printf("E ");
            break;

        case SHF_MASKPROC:
            printf("M ");
            break;
        
        default:
            printf("i ");
            break;
        }

        printf("%x", shdr[i].sh_link);
        printf("%x", shdr[i].sh_info);
        printf("%x", shdr[i].sh_addralign);
    }
    
}
