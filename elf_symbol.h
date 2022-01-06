#ifndef __ELF_SYMBOL_H__
#define __ELF_SYMBOL_H__

#include "elf_types.h"

#define STN_UNDEF 0

/*
    Plan d'attaque de la table des symboles

    1:  Chopper l'adresse de la table des symboles (dans l'entête de section)
    2:  Chopper les nombre de symbole (TailleTotalSymbole / TailleEntreDeSymbole) (dans l'entête de section)
    3:  Répéter pour le nombre de symbole -> lecture, ...
*/


/* ELF32 Structure de la table des symboles en 32-bit (Couleur cf Schéma) */
typedef struct {
    Elf32_Word st_name;     /* (4 octets ORANGE)    Symbol name             */  // NOM Symbole          CF STRING TABLE
    Elf32_Addr st_value;    /* (4 octets ROUGE)     Symbol value            */  // Valeur Symbole
    Elf32_Word st_size;     /* (4 octets BLEU)      Symbol size             */  // Taille Symbole
    unsigned char st_info;  /* (1 octets ROSE)      Symbol type and binding */  // Attribut Symbole
    unsigned char st_other; /* (1 octets VERT)      Symbol visibility       */  // Attribut Symbole
    Elf32_Half st_shndx;    /* (2 octets JAUNE)     Section index           */  // Attribut Symbole
} Elf32_Sym;

/* st_info */
#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

/* st_other */
#define ELF32_ST_VISIBILITY(o) ((o)&0x3)

/* Symbol Binding, ELF32_ST_BIND */
#define STB_LOCAL   0
#define STB_GLOBAL  1
#define STB_WEAK    2
#define STB_LOPROC  13
#define STB_HIPROC  15

/* Symbol Types, ELF32_ST_TYPE */
#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_LOPROC  13
#define STT_HIPROC  15

/* Symbol Visibility */
#define STV_DEFAULT     0
#define STV_INTERNAL    1
#define STV_HIDDEN      2
#define STV_PROTECTED   3

#endif
