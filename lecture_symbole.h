#ifndef __LECTURE_SYMBOLE_H__
#define __LECTURE_SYMBOLE_H__

#include "elf_header.h"
#include "section_header.h"
#include "elf_types.h"
#include "elf_symbol.h"

/**
 * @brief Fonction qui crée une structure contenant la table des symboles
 *
 * @param nom_fichier (char *) Nom du fichier ELF
 * @param ehdr (Elf32_Ehdr *) Structure contenant l'entête pour obtenir le type de l'endian
 * @param shdr (Elf32_Shdr *) Structure contenant l'adresse pour le début de la section
 * @return (Elf32_Sym *) Renvoie une Structure correspondant à la table des symboles
 */
Elf32_Sym *lire_symbole(char *nom_fichier, Elf32_Ehdr ehdr, Elf32_Shdr *shdr);

/**
 * @brief Fonction qui affiche la table des symboles corresondant à un fichier ELF
 *
 * @param sym (Elf32_Sym *) Structure contenant les symboles
 * @param nom_fichier (char *) Nom du fichier ELF
 */
void afficher_symboles(Elf32_Sym *sym, char *nom_fichier, char *shstrtab);

#endif
