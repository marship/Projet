#ifndef __FONCTIONS_UTILITAIRES_H__
#define __FONCTIONS_UTILITAIRES_H__

/**
 * @brief 
 * 
 * TO DO Description
 * 
 * @param nom_fichier 
 */
long hex2dec (char hex[]);

/**
 * @brief 
 * 
 * TO DO Description
 * 
 * @param nom_fichier 
 */
int read_uint16(uint16_t *ptr, FILE *stream, unsigned char endian);

/**
 * @brief 
 * 
 * TO DO Description
 * 
 * @param nom_fichier 
 */
int read_uint32(uint32_t *ptr, FILE *stream, unsigned char endian);

#endif