#ifndef __FONCTIONS_UTILITAIRES_H__
#define __FONCTIONS_UTILITAIRES_H__

#include "elf_types.h"

/**
 * @brief Fonction qui convertit une chaîne de caractère (représentant une valeure hexadécimale) en décimal
 *
 * @param hex (char []) Chaîne de caractère (représentant une valeure hexadécimale)
 * @return (long) Valeure en décimal
 */
long hex2dec(char hex[]);

/**
 * @brief Fonction qui lit un fichier binaire en 16 bits suivant le type d'endian sélectionné
 *
 * @param ptr (uint16_t) Pointeur pour sauvegarder ce que l'on lit
 * @param stream (FILE) Fichier lu
 * @param endian (unsigned char) Invalide = 0 | Little Endian = 1 | Big Endian = 1
 * @return (int) int Si la lecture echoue renvoie 0 | Si la lecture fonctionne renvoie 1
 */
int read_uint16(uint16_t *ptr, FILE *stream, unsigned char endian);

/**
 * @brief Fonction qui lit un fichier binaire en 32 bits suivant le type d'endian sélectionné
 *
 * @param ptr (uint32_t) Pointeur pour sauvegarder ce que l'on lit
 * @param stream (FILE) Fichier lu
 * @param endian (unsigned char) Invalide = 0 | Little Endian = 1 | Big Endian = 1
 * @return (int) Si la lecture echoue renvoie 0 | Si la lecture fonctionne renvoie 1
 */
int read_uint32(uint32_t *ptr, FILE *stream, unsigned char endian);

/**
 * @brief Fonction qui lit un fichier binaire en 32 bits suivant le type d'endian sélectionné
 *
 * @param ptr (int32_t) Pointeur pour sauvegarder ce que l'on lit
 * @param stream (FILE) Fichier lu
 * @param endian (unsigned char) Invalide = 0 | Little Endian = 1 | Big Endian = 1
 * @return (int) Si la lecture echoue renvoie 0 | Si la lecture fonctionne renvoie 1
 */
int read_int32(int32_t *ptr, FILE *stream, unsigned char endian);

/**
 * @brief Fonction qui renvoie le nombre de bits à 1 dans un mot
 * 
 * @param mot (Elf32_Word) Mot dans lequel compter le nombre de bits à 1
 * @return (int) Renvoie le nombre de bits à 1 dans mot
 */
int compter_bits(Elf32_Word mot);

/**
 * @brief Fonction qui lit l'extension du fichier et renvoie si c'est un executable ou non
 * 
 * @param nom (char *) Nom du Fichier (extension inclu)
 * @return (int) Si le fichier n'a pas d'extension, renvoie 0 | Si il en a une, renvoie 1
 */
int extension_fichier(char *nom);

#endif