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
        if( (shdr[j].sh_type == SHT_NOBITS) || (shdr[j].sh_type == SHT_RELA) || (shdr[j].sh_type == SHT_REL) ){
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
        if( (shdr[i].sh_type == SHT_NOBITS) || (shdr[i].sh_type == SHT_RELA) || (shdr[i].sh_type == SHT_REL) ){
            while( (i+j != ehdr.e_shnum) && (shdr[i+j].sh_type == SHT_NOBITS) && (shdr[i+j].sh_type == SHT_RELA) && (shdr[i+j].sh_type == SHT_REL) ){
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
