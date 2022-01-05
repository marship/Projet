#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fonctions_utilitaires.h"
#include <string.h>
#include "elf_symbol.h"

void lire_symbole(char *nom_fichier)
{
    FILE *Handle = fopen(nom_fichier, "rb");

    if (Handle == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    // uint16_t = Paquets de 4 valeurs hexa (1 ligne) /// uint8_t = Paquets de 2 valeurs hexa (1/2 ligne)
    uint8_t adrTableSymbole = 4;
    int nbSymbole = 8;
    char endian = 1;

    // Tableau qui va reçevoir l'int convertit en string
    char buff[20];

    // Convertit l'entier passé en paramètre en un string
    sprintf(buff, "%d", adrTableSymbole);

    // Convertion du string en hexa puis positionnement à l'adresse correspondant à l'hexa
    fseek(Handle, hex2dec(buff), SEEK_SET);

    printf("\n");
    printf("[Nr]\tName\tType\tAddr\tSize\n");

    printf("[0]\tNULL\tNULL\t000000\t000000\n");

    // Lecture des valeurs souhaitées
    for (int i = 1; i < nbSymbole; i++)
    {
        Elf32_Sym sym;

        if (read_uint32(&sym.st_name, Handle, endian) == 0 
        || read_uint32(&sym.st_value, Handle, endian) == 0 
        || read_uint32(&sym.st_size, Handle, endian) == 0 
        || fread(&sym.st_info, sizeof(char), 1, Handle) == 0 
        || fread(&sym.st_other, sizeof(char), 1, Handle) == 0 
        || read_uint16(&sym.st_shndx, Handle, endian) == 0)
        {
            fprintf(stderr, "Error: %s: Failed to read file header\n", nom_fichier);
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

    fclose(Handle);
}
