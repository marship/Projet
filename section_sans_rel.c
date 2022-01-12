#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fonctions_utilitaires.h"
#include "string_table.h"
#include "section_sans_rel.h"

uint32_t nb_rel(Elf32_Ehdr ehdr, Elf32_Shdr *shdr){
    uint32_t i = 0;
    uint32_t j = 0;
    while(j != ehdr.e_shnum){
        if( (shdr[j].sh_type == SHT_RELA) || (shdr[j].sh_type == SHT_REL) ){
            i++;
        }
        j++;
    }
    return i;
}

Elf32_Shdr *maj_section(Elf32_Ehdr ehdr, Elf32_Shdr *shdr){
    uint32_t i = 0;
    uint32_t j = 1;
    while(i != ehdr.e_shnum){
        if( (shdr[i].sh_type == SHT_RELA) || (shdr[i].sh_type == SHT_REL) ){
            while( (i+j != ehdr.e_shnum) && (shdr[i+j].sh_type == SHT_RELA) && (shdr[i+j].sh_type == SHT_REL) ){
                j++;
            }
            if(i+j == ehdr.e_shnum){
                ehdr.e_shnum = i-1;
                return shdr;
            }
            shdr[i] = shdr[i+j];
            j = i;
            i++;
            while(j+1 != ehdr.e_shnum){
                shdr[j] = shdr[j+1];
                j++;
            }
            ehdr.e_shnum--;
            j = 1;
        }
        else{
            i++;
        }
    }
    return shdr;
}

uint32_t index_string(Elf32_Ehdr ehdr, Elf32_Shdr *shdr){
    uint32_t i = 0;
    while(i != ehdr.e_shnum && shdr[i].sh_type != SHT_STRTAB){
        i++;
    }
    if(i == ehdr.e_shnum){
        return 0;
    }
    else{
        return i+1;
    }
}

Elf32_Sym *maj_ndx(Elf32_Sym *sym, Elf32_Shdr *shdr, Elf32_Ehdr ehdr){
    // Recherche de la section des symboles
    int noSection = 0;

    while (noSection < ehdr.e_shnum && shdr[noSection].sh_type != SHT_SYMTAB)
    {
        noSection++;
    }

    if (noSection == ehdr.e_shnum)
    {
        fprintf(stderr, "Erreur: symbol table absente\n");
        exit(EXIT_FAILURE);
    }
    
    // Calcul du nombre de symboles
    int nbSym = shdr[noSection].sh_size / shdr[noSection].sh_entsize;

    int i = 1;
    uint32_t j = 1;
    uint32_t k = 0;
    while(i != nbSym){
        while(j != sym[i].st_shndx){
            if( (shdr[j].sh_type == SHT_RELA) || (shdr[j].sh_type == SHT_REL) ){
                k++;
            }
            j++;
        }
        j = 1;
        //printf("%d / %d\n", sym[i].st_shndx, k);
        sym[i].st_shndx = sym[i].st_shndx - k;
        k = 0;
        i++;
    }
    return sym;
}
