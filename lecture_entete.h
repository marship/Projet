#ifndef __LECTURE_ENTETE_H__
#define __LECTURE_ENTETE_H__

#include "elf_header.h"

/**
 * @brief 
 * 
 * TO DO Description
 * 
 * @param nom_fichier 
 */
Elf32_Ehdr lire_entete(char *nom_fichier);

/**
 * @brief 
 * 
 * TO DO Description
 * 
 * @param hdr
 */
void afficher_entete(Elf32_Ehdr hdr);

#endif