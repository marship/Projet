#ifndef __SECTION_H__
#define __SECTION_H__

#include "section_header.h"


/**
 * @brief Fonction qui renvoie un tableau d'octets correspondant au contenu de la section passée en paramètre
 *
 * @param f (FILE *) Fichier ELF ouvert
 * @param shdr (Elf32_Shdr *) Tableau contenant les en-têtes des sections
 * @param num (int) Indice de la section à lire
 * @return (unsigned char *) Tableau d'octets contenant le contenu de la section lue
 */
unsigned char *lire_section(FILE *f, Elf32_Shdr *shdr, int num);

/**
 * @brief Fonction qui affiche le contenu de la section passée en paramètre
 *
 * @param sect (unsigned char *) Tableau d'octets contenant le contenu de la section à afficher
 * @param shstrtab (char *) Table des chaines de caractères des en-têtes de sections
 * @param shdr (Elf32_Shdr *) Tableau contenant les en-têtes des sections
 * @param index (uint32_t) Indice de l'en-tête de la section à afficher
 * @param shnum (Elf32_Half) Nombre total de sections du fichier
 */
void afficher_section(unsigned char *sect, char *shstrtab, Elf32_Shdr *shdr, uint32_t index, Elf32_Half shnum);


#endif
