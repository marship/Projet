#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fonctions_utilitaires.h"
#include "elf_header.h"

long hex2dec (char hex[])
{
    size_t i = 0;
    long dec = 0;
    long temp = 0;

    while (hex[i] != '\0')
    {
        const int isHEXA = (hex[i] >= 'A') && (hex[i] <= 'F');
        const int ishexa = (hex[i] >= 'a') && (hex[i] <= 'f');
        const int isdigit = (hex[i] >= '0') && (hex[i] <= '9');

        if (ishexa)
        {
            temp = hex[i] - 'a' + 10;
        }
        else if (isHEXA)
        {
            temp = hex[i] - 'A' + 10;
        }
        else if (isdigit)
        {
            temp = hex[i] - '0';
        }
        dec = dec * 16 + temp;
        i++;
    }
    return dec;
}

int read_uint16(uint16_t *ptr, FILE *stream, unsigned char endian) {
    if (endian < ELFDATA2LSB || endian > ELFDATA2MSB) {
        return fread(ptr, sizeof(uint16_t), 1, stream);
    }

    unsigned char b1, b2;

    if (fread(&b1, sizeof(char), 1, stream) == 0 || fread(&b2, sizeof(char), 1, stream) == 0) {
        return 0;
    }

    if (endian == ELFDATA2LSB) {
        *ptr = ((uint16_t) b2) << 8 | (uint16_t) b1;
    }
    else {
        *ptr = ((uint16_t) b1) << 8 | (uint16_t) b2;
    }
    return 1;
}

int read_uint32(uint32_t *ptr, FILE *stream, unsigned char endian) {
    if (endian < ELFDATA2LSB || endian > ELFDATA2MSB) {
        return fread(ptr, sizeof(uint32_t), 1, stream);
    }

    unsigned char b1, b2, b3, b4;

    if (fread(&b1, sizeof(char), 1, stream) == 0 || fread(&b2, sizeof(char), 1, stream) == 0
    || fread(&b3, sizeof(char), 1, stream) == 0 || fread(&b4, sizeof(char), 1, stream) == 0) {
        return 0;
    }

    if (endian == ELFDATA2LSB) {
        *ptr = ((uint32_t) b4) << 24 | ((uint32_t) b3) << 16 | ((uint32_t) b2) << 8 | (uint32_t) b1;
    }
    else {
        *ptr = ((uint32_t) b1) << 24 | ((uint32_t) b2) << 16 | ((uint32_t) b3) << 8 | (uint32_t) b4;
    }
    return 1;
}

int read_int32(int32_t *ptr, FILE *stream, unsigned char endian) {
    if (endian < ELFDATA2LSB || endian > ELFDATA2MSB) {
        return fread(ptr, sizeof(int32_t), 1, stream);
    }

    unsigned char b1, b2, b3, b4;

    if (fread(&b1, sizeof(char), 1, stream) == 0 || fread(&b2, sizeof(char), 1, stream) == 0
    || fread(&b3, sizeof(char), 1, stream) == 0 || fread(&b4, sizeof(char), 1, stream) == 0) {
        return 0;
    }

    if (endian == ELFDATA2LSB) {
        *ptr = ((int32_t) b4) << 24 | ((int32_t) b3) << 16 | ((int32_t) b2) << 8 | (int32_t) b1;
    }
    else {
        *ptr = ((int32_t) b1) << 24 | ((int32_t) b2) << 16 | ((int32_t) b3) << 8 | (int32_t) b4;
    }
    return 1;
}
