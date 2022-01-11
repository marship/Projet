#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "symbol.h"
#include "fonctions_utilitaires.h"
#include "string_table.h"

// Numéro de Section
int noSection = 0;

// Nombre de Symbole
int nbSym = 0;


Elf32_Sym *lire_symboles(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *shdr)
{
    if (shdr == NULL) {
        return NULL;
    }

    // Recherche de la section des symboles
    while (shdr[noSection].sh_type != SHT_SYMTAB && noSection < ehdr.e_shnum)
    {
        noSection++;
    }

    if (noSection == ehdr.e_shnum)
    {
        fprintf(stderr, "ERREUR: table des symboles absente\n");
        exit(EXIT_FAILURE);
    }

    // Calcul du nombre de symboles
    nbSym = shdr[noSection].sh_size / shdr[noSection].sh_entsize;

    // Allocation de la place nécessaire
    Elf32_Sym *sym = malloc(nbSym * sizeof(Elf32_Sym));

    if (sym == NULL) {
        fprintf(stderr, "ERREUR: Impossible d'allouer de la mémoire pour la table de symboles\n");
        exit(EXIT_FAILURE);
    }

    // Deplacement au début des symboles
    if (fseek(f, shdr[noSection].sh_offset, SEEK_SET) != 0)
    {
        fprintf(stderr,
                "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour la table des symboles\n",
                shdr[noSection].sh_size);
        exit(EXIT_FAILURE);
    }

    // Lecture des valeurs souhaitées
    for (int i = 0; i < nbSym; i++)
    {
        if (read_uint32(&sym[i].st_name, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&sym[i].st_value, f, ehdr.e_ident[EI_DATA]) == 0 ||
            read_uint32(&sym[i].st_size, f, ehdr.e_ident[EI_DATA]) == 0 ||
            fread(&sym[i].st_info, sizeof(char), 1, f) == 0 ||
            fread(&sym[i].st_other, sizeof(char), 1, f) == 0 ||
            read_uint16(&sym[i].st_shndx, f, ehdr.e_ident[EI_DATA]) == 0)
        {
            fprintf(stderr,
                    "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour la table des symboles\n",
                    shdr[noSection].sh_size);
            exit(EXIT_FAILURE);
        }
    }
    return sym;
}


void afficher_symboles(Elf32_Sym *sym, Elf32_Ehdr ehdr, Elf32_Shdr *shdr, char *shstrtab, char *strtab)
{
    if (sym == NULL || shdr == NULL || shstrtab == NULL || strtab == NULL) {
        printf("\nDynamic symbol information is not available for displaying symbols.\n");
        return;
    }

    int entries = shdr[noSection].sh_size / shdr[noSection].sh_entsize;

    printf("\nSymbol table '");
    afficher_chaine(shstrtab, shdr[noSection].sh_name, 0);
    printf("' contains %d", entries);
    if (entries == 1) {
        printf(" entry:\n");
    }
    else {
        printf(" entries:\n");
    }

    printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

    for (int i = 0; i < nbSym; i++)
    {
        printf("%6d: ", i);                 // Affichage du numéro du symbole
        printf("%.8x ", sym[i].st_value);  // Affichage de la valeur
        printf("%5x ", sym[i].st_size);    // Affichage de la taille

        switch (ELF32_ST_TYPE(sym[i].st_info)) // Affichage du type
        {
        case STT_NOTYPE:
            printf("NOTYPE  ");
            break;

        case STT_OBJECT:
            printf("OBJECT  ");
            break;

        case STT_FUNC:
            printf("FUNC    ");
            break;

        case STT_SECTION:
            printf("SECTION ");
            break;

        case STT_FILE:
            printf("FILE    ");
            break;

        case STT_LOPROC:
            printf("LOPROC  ");
            break;

        case STT_HIPROC:
            printf("HIPROC  ");
            break;

        default:
            printf("<processor specific>: %d ", ELF32_ST_TYPE(sym[i].st_info));
            break;
        }

        switch (ELF32_ST_BIND(sym[i].st_info)) // Affichage du lien
        {
        case STB_LOCAL:
            printf("LOCAL  ");
            break;

        case STB_GLOBAL:
            printf("GLOBAL ");
            break;

        case STB_WEAK:
            printf("WEAK   ");
            break;

        case STB_LOPROC:
            printf("LOPROC ");
            break;

        case STB_HIPROC:
            printf("HIPROC ");
            break;

        default:
            printf("<processor specific>: %d ", ELF32_ST_BIND(sym[i].st_info));
            break;
        }

        switch (ELF32_ST_VISIBILITY(sym[i].st_other)) // Affichage de la visibilité
        {
        case STV_DEFAULT:
            printf("DEFAULT ");
            break;

        case STV_INTERNAL:
            printf("INTERNAL ");
            break;

        case STV_HIDDEN:
            printf("HIDDEN  ");
            break;

        case STV_PROTECTED:
            printf("PROTECTED ");
            break;

        default:
            printf("[<other>: %d] ", ELF32_ST_VISIBILITY(sym[i].st_other));
            break;
        }

        // Affichage du ndx
        switch (sym[i].st_shndx)
        {
        case SHN_UNDEF:
            printf(" UND ");
            break;
        case SHN_ABS:
            printf(" ABS ");
            break;
        case SHN_COMMON:
            printf(" COM ");
            break;
        default:
            if (sym[i].st_shndx >= 0xff00 && sym[i].st_shndx <= 0xff1f) {
                printf("PRC[0x%4x] ", sym[i].st_shndx);
            }
            else if (sym[i].st_shndx > 0xff1f) {
                printf("RSV[0x%4x] ", sym[i].st_shndx);
            }
            else if (sym[i].st_shndx >= ehdr.e_shnum) {
                printf("bad section index[%3d] ", sym[i].st_shndx);
            }
            else {
                printf("%4d ", sym[i].st_shndx);
            }
        }

        afficher_chaine(strtab, sym[i].st_name, 0);
        printf("\n");
    }
}
