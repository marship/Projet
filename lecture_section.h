#ifndef __LECTURE_SECTION_H__
#define __LECTURE_SECTION_H__

#include "elf_header.h"
#include "section_header.h"
#include "elf_types.h"

/**
 * @brief Fonction qui crée une structure contenant la table des symboles
 *
 * @param nom_fichier (char *) Nom du fichier ELF
 * @param ehdr (Elf32_Ehdr *) Structure contenant l'entête pour obtenir le type de l'endian
 * @param shdr (Elf32_Shdr *) Structure contenant l'adresse pour le début de la section
 * @return (Elf32_Sym *) Renvoie une Structure correspondant à la table des symboles
 */
Elf32_Shdr *lire_section(char *nom_fichier, Elf32_Ehdr ehdr);

/**
 * @brief Fonction qui affiche la table des symboles corresondant à un fichier ELF
 *
 * @param sym (Elf32_Sym *) Structure contenant les sections
 */
void afficher_section(Elf32_Shdr *shdr, Elf32_Ehdr ehdr, char *shstrtab);

#endif
