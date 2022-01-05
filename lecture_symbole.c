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
    printf("[Nr] Name\t\t\tType\t\t\tAddr\t\t\tOff\t\t\tSize\t\t\tES\tFlg\tLk\tInf\tAl\t\n");

    printf("[ 0] \t\t\t\tNULL\t\t\t00000000\t\t000000\t\t\t000000\t\t\t00\t\t0\t0\t0\t\n");

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

        printf("[%d] Name\t\t\tType\t\t\tAddr\t\t\tOff\t\t\tSize\t\t\tES\tFlg\tLk\tInf\tAl\t\n", i);
        // TO DO !!!
    }

    fclose(Handle);
}