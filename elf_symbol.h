#ifndef __ELF_SYMBOL_H__
#define __ELF_SYMBOL_H__

#include "elf_types.h"

#define STN_UNDEF 0

/* ELF32 Structure de la table des symboles en 32-bit */
typedef struct {
    Elf32_Word st_name;
    Elf32_Addr st_value;
    Elf32_Word st_size;
    unsigned char st_info;
    unsigned char st_other;
    Elf32_Half st_shndx;
} Elf32_Sym;

/* st_info */
#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

/* st_other */
#define ELF32_ST_VISIBILITY(o) ((o)&0x3)
#define ELF64_ST_VISIBILITY(o) ((o)&0x3)

/* Symbol Binding, ELF32_ST_BIND */
#define STB_LOCAL 0
#define STB_GLOBAL 1
#define STB_WEAK 2
#define STB_LOPROC 13
#define STB_HIPROC 15

/* Symbol Types, ELF32_ST_TYPE */
#define STT_NOTYPE 0
#define STT_OBJECT 1
#define STT_FUNC 2
#define STT_SECTION 3
#define STT_FILE 4
#define STT_LOPROC 13
#define STT_HIPROC 15

/* Symbol Table Entry: Index 0 */
#define st_name 0 // No name
#define st_value 0 // Zero value
#define st_size 0 // No size
#define st_info 0 // No type, local binding
#define st_other 0
#define st_shndx // SHN_UNDEF No section

/* Symbol Visibility */
#define STV_DEFAULT 0
#define STV_INTERNAL 1
#define STV_HIDDEN 2
#define STV_PROTECTED 3

#endif
