#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lecture_symbole.h"
#include "fonctions_utilitaires.h"

void lire_symbole(char *nom_fichier, Elf32_Ehdr ehdr, Elf32_Shdr *shdr)
{
    FILE *f;
    int i = 0;
    while(shdr[i].sh_name != ".symtab"){
        i++;
    }

    // Ouverture du fichier
    f = fopen(nom_fichier, "rb");

    if (f == NULL) {
        fprintf(stderr, "Impossible d'ouvir le fichier : %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    if (fseek(f, shdr[i].sh_addr + shdr[i].sh_offset, SEEK_SET) != 0) {
        fprintf(stderr, "Erreur de lecture du fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    printf("[Nr]\tName\tType\tAddr\tSize\n");
    printf("[0]\tNULL\tNULL\t000000\t000000\n");

    int nbSym = shdr[i].sh_size / shdr[i].sh_entsize;
    // Lecture des valeurs souhaitées
    for (int i = 1; i < nbSym; i++)
    {
        Elf32_Sym sym;

        if (read_uint32(&sym.st_name, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&sym.st_value, f, ehdr.e_ident[EI_DATA]) == 0 
        || read_uint32(&sym.st_size, f, ehdr.e_ident[EI_DATA]) == 0 
        || fread(&sym.st_info, sizeof(char), 1, f) == 0 
        || fread(&sym.st_other, sizeof(char), 1, f) == 0 
        || read_uint16(&sym.st_shndx, f, ehdr.e_ident[EI_DATA]) == 0)
        {
            fprintf(stderr, "Erreur: %s: Echec de la lecteur du symbole\n", nom_fichier);
            exit(EXIT_FAILURE);
        }

        printf("[%d]\t", i);
        printf("Name\t");

        switch (ELF32_ST_TYPE(sym.st_info))
        {
        case STT_NOTYPE:
            printf("Sans\t");
            break;
        
        case STT_OBJECT:
            printf("Objet\t");
            break;

        case STT_FUNC:
            printf("Fonction\t");
            break;

        case STT_SECTION:
            printf("Section\t");
            break;

        case STT_FILE:
            printf("File\t");
            break;

        case STT_LOPROC:
            printf("Loproc\t");
            break;

        case STT_HIPROC:
            printf("Hiproc\t");
            break;
        
        default:
            printf("Inconnu\t");
            break;
        }

        printf("%x\t", sym.st_value);
        printf("Off");
        printf("%d\n", sym.st_size);
        /*
        switch (ELF32_ST_BIND(sym.st_info))
        {
        case STB_LOCAL:
            printf("Local\t");
            break;
        
        case STB_GLOBAL:
            printf("Global\t");
            break;

        case STB_WEAK:
            printf("Weak\t");
            break;

        case STB_LOPROC:
            printf("Loproc\t");
            break;

        case STB_HIPROC:
            printf("Hiproc\t");
            break;
        
        default:
            break;
        } */

        //printf("[%d] Name\t\tType\t\tAddr\t\tOff\t\tSize\t\tES\tFlg\tLk\tInf\tAl\t\n", i);
        // TO DO !!!
    }

    fclose(f);
}
