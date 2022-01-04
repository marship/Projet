#ifndef __SECTION_HEADER_H__
#define __SECTION_HEADER_H__

#include "elf_types.h"

typedef struct
{
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_Shdr;

/* sh_type */
#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff

/* sh_flags */
#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MASKPROC 0xf0000000

/* Special Section Indexes */
#define SHN_UNDEF 0          /* This value marks an undefined,missing,irrelevant or otherwise meaningless section reference. */
#define SHN_LORESERVE 0xff00 /* This value specifies the lower bound of the range of reserved indexes. */
#define SHN_LOPROC 0xff00    // Values in this inclusive range are reserved
#define SHN_HIPROC 0xff1f    // for processor-specific semantics.
#define SHN_ABS 0xfff1       /* This value specifies absolute values for the corresponding reference. */
#define SHN_COMMON 0xfff2    /* Symbols defined relative to this section are common symbols. */
#define SHN_HIRESERVE 0xffff /* This value specifies the upper bound of the range of reserved indexes. */

#endif
