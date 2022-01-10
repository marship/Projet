#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

#include "elf_header.h"
#include "section_header.h"

/**
 * @brief Fonction qui lit la string table contenant les noms des symboles
 *
 * @param f (FILE *) Fichier ELF ouvert
 * @param shdr (Elf32_Shdr *) Tableau contenant les entêtes des sections
 * @param ehdr (Elf32_Ehdr) Structure contant l'entête du fichier ELF
 * @return (char *) Renvoie tableau de caractères contenant tous les noms des symboles
 */
char *lire_strtab(FILE *f, Elf32_Shdr *shdr, Elf32_Ehdr ehdr);

/**
 * @brief Fonction qui lit la string table contenant les noms des sections
 *
 * @param f (FILE *) Fichier ELF ouvert
 * @param shdr (Elf32_Shdr *) Tableau contenant les entêtes des sections
 * @param ehdr (Elf32_Ehdr) Structure contant l'entête du fichier ELF
 * @return (char *) Renvoie tableau de caractères contenant tous les noms des sections
 */
char *lire_shstrtab(FILE *f, Elf32_Shdr *shdr, Elf32_Ehdr ehdr);

/**
 * @brief Fonction qui affiche une chaine de caractère contenue à l'indice index dans la string table strtab jusqu'à une longueur_max
 *
 * @param strtab (char *) Tableau de caractères contenant tous les noms des symboles ou sections
 * @param index (Elf32_Word) Entier correspondant à l'indice auquel aller chercher la chaine de caractères
 * @param longueur_max (int) Longueur maximum à lire dans la string table (si 0 la chaine est lue entièrement)
 * @return (int) Renvoie la longueur de la chaine affichée
 */
int afficher_chaine(char *strtab, Elf32_Word index, int longueur_max);

#endif