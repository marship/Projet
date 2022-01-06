#ifndef __LECTURE_ENTETE_H__
#define __LECTURE_ENTETE_H__

#include "elf_header.h"

/**
 * @brief Fonction qui renvoie une structure Elf32_Ehdr correspondant à l'entête du fichier ELF passé en paramètre
 *
 * @param nom_fichier (char *) Nom|Chemin du Fichier ELF
 * @return (Elf32_Ehdr) Renvoie une structure Elf32_Ehdr correspondant à l'entête
 */
Elf32_Ehdr lire_entete(char *nom_fichier);

/**
 * @brief Fonction qui affiche une structure Elf32_Ehdr correspondant à l'entête d'un fichier ELF
 *
 * @param hdr Structure Elf32_Ehdr correspondant à l'entête d'un fichier ELF
 */
void afficher_entete(Elf32_Ehdr hdr);

#endif