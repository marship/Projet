#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fonctions_utilitaires.h"
#include <string.h>

void lire_symbole(char *nom_fichier)
{
    FILE *Handle = fopen(nom_fichier, "rb");

    if (Handle == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    // uint16_t = Paquets de 4 valeurs hexa (1 ligne) /// uint8_t = Paquets de 2 valeurs hexa (1/2 ligne)
    uint8_t Temp;

    char snum[] = "4"; // = 4 en décimal
    fseek(Handle, hex2dec(snum), SEEK_SET);

    fread(&Temp, sizeof(uint8_t), 1, Handle);
    printf("Le char à la pos 0 est : %x\n", Temp);

    fread(&Temp, sizeof(uint8_t), 1, Handle);
    printf("Le char à la pos 1 est : %x\n", Temp);

    fread(&Temp, sizeof(uint8_t), 1, Handle);
    printf("Le char à la pos 2 est : %x\n", Temp);

    fread(&Temp, sizeof(uint8_t), 1, Handle);
    printf("Le char à la pos 3 est : %x\n", Temp);

    fread(&Temp, sizeof(uint8_t), 1, Handle);
    printf("Le char à la pos 4 est : %x\n", Temp);

    fread(&Temp, sizeof(uint8_t), 1, Handle);
    printf("Le char à la pos 5 est : %x\n", Temp);

    fread(&Temp, sizeof(uint8_t), 1, Handle);
    printf("Le char à la pos 6 est : %x\n", Temp);

    fread(&Temp, sizeof(uint8_t), 1, Handle);
    printf("Le char à la pos 7 est : %x\n", Temp);

    fclose(Handle);
}