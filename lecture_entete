#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    FILE *fic;
    char e;
    char l;
    char f;
    char class;
    char data;
    char version;
    char pad;
    char nident;
    uint16_t type;
    uint16_t machine;
    uint32_t version2;
    uint32_t entree;
    uint32_t progtete;
    uint32_t secttete;
    uint32_t flags;
    uint16_t taille;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;

    fic = fopen("csapp.o", "rb");

    if(fic == NULL){
        fprintf(stderr, "faux\n");
        exit(EXIT_FAILURE);
    }

    fread(&e, sizeof(char), 1, fic);
    fread(&e, sizeof(char), 1, fic);
    fread(&l, sizeof(char), 1, fic);
    fread(&f, sizeof(char), 1, fic);
    
    fread(&class, sizeof(char), 1, fic);
    printf("Classe : ");
    switch (class)
    {
    case 0:
        printf("invalide !\n");
        break;

    case 1:
        printf("32 bits\n");
        break;

    case 2:
        printf("64 bits\n");
        break;
    
    default:
        break;
    }

    fread(&data, sizeof(char), 1, fic);
    printf("Données : ");
    switch (data)
    {
    case 0:
        printf("invalide !\n");
        break;

    case 1:
        printf("Little endian\n");
        break;

    case 2:
        printf("Big endian\n");
        break;
    
    default:
        break;
    }

    fread(&version, sizeof(char), 1, fic);
    printf("Version : ");
    switch (version)
    {
    case 0:
        printf("invalide !\n");
        break;

    case 1:
        printf("actuelle\n");
        break;
    
    default:
        break;
    }

    
    for (int i = 0; i < 9; i++)
    {
        fread(&pad, sizeof(char), 1, fic);
    }
    fread(&type, sizeof(uint16_t), 1, fic);
    printf("Type : ");
    switch (type)
    {
    case 0:
        printf("Sans type\n");
        break;

    case 1:
        printf("Relogeable\n");
        break;
    
    case 2:
        printf("Executable\n");
        break;
    
    case 3:
        printf("Objet partage\n");
        break;
    
    case 4:
        printf("Core\n");
        break;
    
    default:
        break;
    }

    fread(&machine, sizeof(uint16_t), 1, fic);
    printf("Machine : ");
    switch (machine)
    {
    case 0:
        printf("Sans machine\n");
        break;

    case 1:
        printf("AT&T WE 32100\n");
        break;
    
    case 2:
        printf("SPARC\n");
        break;
    
    case 3:
        printf("Intel Architecture\n");
        break;
    
    case 4:
        printf("Motorola 68000\n");
        break;
    
    case 5:
        printf("Motorola 88000\n");
        break;
    
    case 7:
        printf("Intel 80860\n");
        break;
    
    case 8:
        printf("MIPS RS3000 Big-Endian\n");
        break;
    
    case 10:
        printf("MIPS RS4000 Big-Endian\n");
        break;

    case 62:
        printf("Advanced Micro Devices X86-64\n");
        break;
    
    default:
        break;
    }

    fread(&version2, sizeof(uint32_t), 1, fic);
    printf("Version : ");
    switch (version2)
    {
    case 0:
        printf("invalide !\n");
        break;

    case 1:
        printf("actuelle\n");
        break;
    
    default:
        break;
    }


    fread(&entree, sizeof(uint32_t), 1, fic);
    printf("Adresse du point d'entrée : 0x%d\n", entree);

    fread(&progtete, sizeof(uint32_t), 1, fic);
    printf("Adresse de l'en-tête du programme : %d\n", progtete);

    fread(&pad, sizeof(uint32_t), 1, fic);
    fread(&flags, sizeof(uint32_t), 1, fic);
    fread(&secttete, sizeof(uint16_t), 1, fic);
    printf("Début de l'en-tête de la section : %d\n", secttete);
    printf("Adresse des flags : 0x%d\n", flags);

    fread(&pad, sizeof(uint16_t), 1, fic);
    fread(&pad, sizeof(uint16_t), 1, fic);
    fread(&pad, sizeof(uint16_t), 1, fic);
    fread(&pad, sizeof(uint16_t), 1, fic);
    fread(&pad, sizeof(uint16_t), 1, fic);
    fread(&taille, sizeof(uint16_t), 1, fic);
    printf("Taille : %d\n", taille);

    fread(&phentsize, sizeof(uint16_t), 1, fic);
    printf("Taille de l'en-tête du programme : %d\n", phentsize);

    fread(&phnum, sizeof(uint16_t), 1, fic);
    printf("Nombre d'en-tête du programme : %d\n", phnum);

    fread(&shentsize, sizeof(uint16_t), 1, fic);
    printf("Taille de l'en-tête des sections : %d\n", shentsize);

    fread(&shnum, sizeof(uint16_t), 1, fic);
    printf("Nombre d'en-tête des sections : %d\n", shnum);

    fread(&shstrndx, sizeof(uint16_t), 1, fic);
    printf("Table d'index des chaînes d'en-tête de section : %d\n", shstrndx);

    fclose(fic);
    return 0;
}
