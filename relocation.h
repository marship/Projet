#ifndef __RELOCATION_H__
#define __RELOCATION_H__

#include "elf_types.h"
#include "elf_header.h"
#include "section_header.h"
#include "symbol.h"

/* ELF32 Structure de la table des symboles en 32-bit */
typedef struct {
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
} Elf32_Rel;

typedef struct {
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
    Elf32_Sword r_addend;
} Elf32_Rela;

typedef union {
    Elf32_Rel *rel;
    Elf32_Rela *rela;
} Relocations;

/* r_info */
#define ELF32_R_SYM(i)    ((i) >> 8)
#define ELF32_R_TYPE(i)   ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s) << 8) + (unsigned char)(t))

/* Relocation types */
#define R_ARM_NONE               0
#define R_ARM_PC24               1
#define R_ARM_ABS32              2
#define R_ARM_REL32              3
#define R_ARM_LDR_PC_G0          4
#define R_ARM_ABS16              5
#define R_ARM_ABS12              6
#define R_ARM_THM_ABS5           7
#define R_ARM_ABS8               8
#define R_ARM_SBREL32            9
#define R_ARM_THM_CALL           10
#define R_ARM_THM_PC8            11
#define R_ARM_BREL_ADJ           12
#define R_ARM_TLS_DESC           13
#define R_ARM_THM_SWI8           14
#define R_ARM_XPC25              15
#define R_ARM_THM_XPC22          16
#define R_ARM_TLS_DTPMOD32       17
#define R_ARM_TLS_DTPOFF32       18
#define R_ARM_TLS_TPOFF32        19
#define R_ARM_COPY               20
#define R_ARM_GLOB_DAT           21
#define R_ARM_JUMP_SLOT          22
#define R_ARM_RELATIVE           23
#define R_ARM_GOTOFF32           24
#define R_ARM_BASE_PREL          25
#define R_ARM_GOT_BREL           26
#define R_ARM_PLT32              27
#define R_ARM_CALL               28
#define R_ARM_JUMP24             29
#define R_ARM_THM_JUMP24         30
#define R_ARM_BASE_ABS           31
#define R_ARM_ALU_PCREL_7_0      32
#define R_ARM_ALU_PCREL_15_8     33
#define R_ARM_ALU_PCREL_23_15    34
#define R_ARM_LDR_SBREL_11_0_NC  35
#define R_ARM_ALU_SBREL_19_12_NC 36
#define R_ARM_ALU_SBREL_27_20_CK 37
#define R_ARM_TARGET1            38
#define R_ARM_SBREL31            39
#define R_ARM_V4BX               40
#define R_ARM_TARGET2            41
#define R_ARM_PREL31             42
#define R_ARM_MOVW_ABS_NC        43
#define R_ARM_MOVT_ABS           44
#define R_ARM_MOVW_PREL_NC       45
#define R_ARM_MOVT_PREL          46
#define R_ARM_THM_MOVW_ABS_NC    47
#define R_ARM_THM_MOVT_ABS       48
#define R_ARM_THM_MOVW_PREL_NC   49
#define R_ARM_THM_MOVT_PREL      50
#define R_ARM_THM_JUMP19         51
#define R_ARM_THM_JUMP6          52
#define R_ARM_THM_ALU_PREL_11_0  53
#define R_ARM_THM_PC12           54
#define R_ARM_ABS32_NOI          55
#define R_ARM_REL32_NOI          56
#define R_ARM_ALU_PC_G0_NC       57
#define R_ARM_ALU_PC_G0          58
#define R_ARM_ALU_PC_G1_NC       59
#define R_ARM_ALU_PC_G1          60
#define R_ARM_ALU_PC_G2          61
#define R_ARM_LDR_PC_G1          62
#define R_ARM_LDR_PC_G2          63
#define R_ARM_LDRS_PC_G0         64
#define R_ARM_LDRS_PC_G1         65
#define R_ARM_LDRS_PC_G2         66
#define R_ARM_LDC_PC_G0          67
#define R_ARM_LDC_PC_G1          68
#define R_ARM_LDC_PC_G2          69
#define R_ARM_ALU_SB_G0_NC       70
#define R_ARM_ALU_SB_G0          71
#define R_ARM_ALU_SB_G1_NC       72
#define R_ARM_ALU_SB_G1          73
#define R_ARM_ALU_SB_G2          74
#define R_ARM_LDR_SB_G0          75
#define R_ARM_LDR_SB_G1          76
#define R_ARM_LDR_SB_G2          77
#define R_ARM_LDRS_SB_G0         78
#define R_ARM_LDRS_SB_G1         79
#define R_ARM_LDRS_SB_G2         80
#define R_ARM_LDC_SB_G0          81
#define R_ARM_LDC_SB_G1          82
#define R_ARM_LDC_SB_G2          83
#define R_ARM_MOVW_BREL_NC       84
#define R_ARM_MOVT_BREL          85
#define R_ARM_MOVW_BREL          86
#define R_ARM_THM_MOVW_BREL_NC   87
#define R_ARM_THM_MOVT_BREL      88
#define R_ARM_THM_MOVW_BREL      89
#define R_ARM_TLS_GOTDESC        90
#define R_ARM_TLS_CALL           91
#define R_ARM_TLS_DESCSEQ        92
#define R_ARM_THM_TLS_CALL       93
#define R_ARM_PLT32_ABS          94
#define R_ARM_GOT_ABS            95
#define R_ARM_GOT_PREL           96
#define R_ARM_GOT_BREL12         97
#define R_ARM_GOTOFF12           98
#define R_ARM_GOTRELAX           99
#define R_ARM_GNU_VTENTRY        100
#define R_ARM_GNU_VTINHERIT      101
#define R_ARM_THM_JUMP11         102
#define R_ARM_THM_JUMP8          103
#define R_ARM_TLS_GD32           104
#define R_ARM_TLS_LDM32          105
#define R_ARM_TLS_LDO32          106
#define R_ARM_TLS_IE32           107
#define R_ARM_TLS_LE32           108
#define R_ARM_TLS_LDO12          109
#define R_ARM_TLS_LE12           110
#define R_ARM_TLS_IE12GP         111
#define R_ARM_PRIVATE_0          112
#define R_ARM_PRIVATE_1          113
#define R_ARM_PRIVATE_2          114
#define R_ARM_PRIVATE_3          115
#define R_ARM_PRIVATE_4          116
#define R_ARM_PRIVATE_5          117
#define R_ARM_PRIVATE_6          118
#define R_ARM_PRIVATE_7          119
#define R_ARM_PRIVATE_8          120
#define R_ARM_PRIVATE_9          121
#define R_ARM_PRIVATE_10         122
#define R_ARM_PRIVATE_11         123
#define R_ARM_PRIVATE_12         124
#define R_ARM_PRIVATE_13         125
#define R_ARM_PRIVATE_14         126
#define R_ARM_PRIVATE_15         127
#define R_ARM_ME_TOO             128
#define R_ARM_THM_TLS_DESCSEQ16  129
#define R_ARM_THM_TLS_DESCSEQ32  130
#define R_ARM_THM_GOT_BREL12     131
#define R_ARM_THM_ALU_ABS_G0_NC  132
#define R_ARM_THM_ALU_ABS_G1_NC  133
#define R_ARM_THM_ALU_ABS_G2_NC  134
#define R_ARM_THM_ALU_ABS_G3     135

#define R_ARM_IRELATIVE          160


/**
 * @brief Fonction qui renvoie un tableau de Relocations correspondant aux ré-adressages à effectuer
 *
 * @param f (FILE *) Fichier ELF ouvert
 * @param ehdr (Elf32_Ehdr) Structure contenant l'en-tête du fichier ELF
 * @param shdr (Elf32_Shdr *) Tableau contenant les en-têtes des sections
 * @return (Relocations) Renvoie un tableau de Relocations contenant des tableaux de Elf32_Rel ou Elf32_Rela
 */
Relocations *lire_relocations(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *shdr);

/**
 * @brief Fonction qui libère la mémoire occupée par un tableau de Relocations
 *
 * @param reloc (Relocations *) Tableau de Relocations à libérer
 * @param ehdr (Elf32_Ehdr) Structure contenant l'en-tête du fichier ELF
 */
void liberer_relocations(Relocations *reloc, Elf32_Ehdr ehdr);

/**
 * @brief Fonction affiche les ré-adressages à effectuer
 *
 * @param reloc (Relocations *) Tableau de Relocations contenant les ré-adressages à effectuer
 * @param ehdr (Elf32_Ehdr) Structure contenant l'en-tête du fichier ELF
 * @param shdr (Elf32_Shdr *) Tableau contenant les en-têtes des sections
 * @param sym (Elf32_Sym *) Table des symboles
 * @param shstrtab (char *) Table des chaines de caractères des sections
 * @param strtab (char *) Table des chaines de caractères des symboles
 */
void afficher_relocations(Relocations *reloc, Elf32_Ehdr ehdr, Elf32_Shdr *shdr, Elf32_Sym *sym,
                          char *shstrtab, char *strtab);


#endif
