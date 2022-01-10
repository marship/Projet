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

int compter_flags(Elf32_Word flags);

#endif