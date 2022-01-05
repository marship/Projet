#ifndef __LECTURE_ENTETE_H__
#define __LECTURE_ENTETE_H__

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

/**
 * @brief 
 * 
 * TO DO Description
 * 
 * @param nom_fichier 
 */
void lire_entete(char *nom_fichier);

#endif