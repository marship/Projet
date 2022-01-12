#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "relocation.h"
#include "fonctions_utilitaires.h"
#include "string_table.h"

void afficher_type_relocation(unsigned char t);


Relocations *lire_relocations(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *shdr) {
    if (shdr == NULL) {
        return NULL;
    }

    Relocations *reloc = malloc(ehdr.e_shnum * sizeof(Relocations));

    if (reloc == NULL) {
        fprintf(stderr, "ERREUR: Impossible d'allouer de la mémoire pour les ré-adressages\n");
        exit(EXIT_FAILURE);
    }

    int nb_reloc = 0;

    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_REL || shdr[i].sh_type == SHT_RELA) {
            if (fseek(f, shdr[i].sh_offset, SEEK_SET) != 0) {
                fprintf(stderr,
                        "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour les ré-adressages\n",
                        shdr[i].sh_size);
                exit(EXIT_FAILURE);
            }

            nb_reloc++;
            int entries = shdr[i].sh_size / shdr[i].sh_entsize;

            if (shdr[i].sh_type == SHT_REL) {
                reloc[i].rel = malloc(entries * sizeof(Elf32_Rel));

                for (int j = 0; j < entries; j++) {
                    if (read_uint32(&(reloc[i].rel[j].r_offset), f, ehdr.e_ident[EI_DATA]) == 0 ||
                        read_uint32(&(reloc[i].rel[j].r_info), f, ehdr.e_ident[EI_DATA]) == 0)
                    {
                        fprintf(stderr,
                                "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour les ré-adressages\n",
                                shdr[i].sh_size);
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else {
                reloc[i].rela = malloc(entries * sizeof(Elf32_Rela));

                for (int j = 0; j < entries; j++) {
                    if (read_uint32(&(reloc[i].rela[j].r_offset), f, ehdr.e_ident[EI_DATA]) == 0 ||
                        read_uint32(&(reloc[i].rela[j].r_info), f, ehdr.e_ident[EI_DATA]) == 0 ||
                        read_int32(&(reloc[i].rela[j].r_addend), f, ehdr.e_ident[EI_DATA]) == 0)
                    {
                        fprintf(stderr,
                                "ERREUR: La lecture de %d octets va au delà de la fin du fichier pour les ré-adressages\n",
                                shdr[i].sh_size);
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    if (nb_reloc == 0) {
        liberer_relocations(reloc, ehdr);
        return NULL;
    }

    return reloc;
}

void liberer_relocations(Relocations *reloc, Elf32_Ehdr ehdr) {
    if (reloc == NULL) {
        return;
    }

    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (reloc[i].rel != NULL) {
            free(reloc[i].rel);
        }
        else if (reloc[i].rela != NULL) {
            free(reloc[i].rela);
        }
    }

    free(reloc);
}

void afficher_relocations(Relocations *reloc, Elf32_Ehdr ehdr, Elf32_Shdr *shdr, Elf32_Sym *sym,
                          char *shstrtab, char *strtab)
{
    if (shdr == NULL || reloc == NULL || sym == NULL) {
        printf("\nThere are no relocations in this file.\n");
        return;
    }

    for (int i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_REL || shdr[i].sh_type == SHT_RELA) {

            int entries = shdr[i].sh_size / shdr[i].sh_entsize;

            printf("\nRelocation section '");
            afficher_chaine(shstrtab, shdr[i].sh_name, 0);
            printf("' at offset 0x%x contains %d ", shdr[i].sh_offset, entries);

            if (entries == 1) {
                printf("entry:\n");
            }
            else {
                printf("entries:\n");
            }

            if (shdr[shdr[i].sh_link].sh_type == SHT_SYMTAB) {
                if (shdr[i].sh_type == SHT_REL) {
                    printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");

                    for (int j = 0; j < entries; j++) {
                        printf("%08x  ", reloc[i].rel[j].r_offset);
                        printf("%08x ", reloc[i].rel[j].r_info);
                        afficher_type_relocation(ELF32_R_TYPE(reloc[i].rel[j].r_info));

                        Elf32_Word k = ELF32_R_SYM(reloc[i].rel[j].r_info);
                        printf("%08x   ", sym[k].st_value);

                        if (ELF32_ST_TYPE(sym[k].st_info) == STT_SECTION) {
                            Elf32_Half l = sym[k].st_shndx;
                            afficher_chaine(shstrtab, shdr[l].sh_name, 0);
                        }
                        else {
                            afficher_chaine(strtab, sym[k].st_name, 0);
                        }
                        printf("\n");
                    }
                }
                else {
                    printf(" Offset     Info    Type            Sym.Value  Sym. Name + Addend\n");

                    for (int j = 0; j < entries; j++) {
                        printf("%08x  ", reloc[i].rela[j].r_offset);
                        printf("%08x ", reloc[i].rela[j].r_info);
                        afficher_type_relocation(ELF32_R_TYPE(reloc[i].rela[j].r_info));

                        Elf32_Word k = ELF32_R_SYM(reloc[i].rela[j].r_info);
                        printf("%08x   ", sym[k].st_value);

                        if (ELF32_ST_TYPE(sym[k].st_info) == STT_SECTION) {
                            Elf32_Half l = sym[k].st_shndx;
                            afficher_chaine(shstrtab, shdr[l].sh_name, 0);
                        }
                        else {
                            afficher_chaine(strtab, sym[k].st_name, 0);
                        }
                        printf(" + %x\n", reloc[i].rela[j].r_addend);
                    }
                }
            }
        }
    }
}

void afficher_type_relocation(unsigned char t) {
    switch (t) {
    case R_ARM_NONE:
        printf("R_ARM_NONE        ");
        break;
    case R_ARM_PC24:
        printf("R_ARM_PC24        ");
        break;
    case R_ARM_ABS32:
        printf("R_ARM_ABS32       ");
        break;
    case R_ARM_REL32:
        printf("R_ARM_REL32       ");
        break;
    case R_ARM_LDR_PC_G0:
        printf("R_ARM_LDR_PC_G0   ");
        break;
    case R_ARM_ABS16:
        printf("R_ARM_ABS16       ");
        break;
    case R_ARM_ABS12:
        printf("R_ARM_ABS12       ");
        break;
    case R_ARM_THM_ABS5:
        printf("R_ARM_THM_ABS5    ");
        break;
    case R_ARM_ABS8:
        printf("R_ARM_ABS8        ");
        break;
    case R_ARM_SBREL32:
        printf("R_ARM_SBREL32     ");
        break;
    case R_ARM_THM_CALL:
        printf("R_ARM_THM_CALL    ");
        break;
    case R_ARM_THM_PC8:
        printf("R_ARM_THM_PC8     ");
        break;
    case R_ARM_BREL_ADJ:
        printf("R_ARM_BREL_ADJ    ");
        break;
    case R_ARM_TLS_DESC:
        printf("R_ARM_TLS_DESC    ");
        break;
    case R_ARM_THM_SWI8:
        printf("R_ARM_THM_SWI8    ");
        break;
    case R_ARM_XPC25:
        printf("R_ARM_XPC25       ");
        break;
    case R_ARM_THM_XPC22:
        printf("R_ARM_THM_XPC22   ");
        break;
    case R_ARM_TLS_DTPMOD32:
        printf("R_ARM_TLS_DTPMOD32 ");
        break;
    case R_ARM_TLS_DTPOFF32:
        printf("R_ARM_TLS_DTPOFF32 ");
        break;
    case R_ARM_TLS_TPOFF32:
        printf("R_ARM_TLS_TPOFF32 ");
        break;
    case R_ARM_COPY:
        printf("R_ARM_COPY        ");
        break;
    case R_ARM_GLOB_DAT:
        printf("R_ARM_GLOB_DAT    ");
        break;
    case R_ARM_JUMP_SLOT:
        printf("R_ARM_JUMP_SLOT   ");
        break;
    case R_ARM_RELATIVE:
        printf("R_ARM_RELATIVE    ");
        break;
    case R_ARM_GOTOFF32:
        printf("R_ARM_GOTOFF32    ");
        break;
    case R_ARM_BASE_PREL:
        printf("R_ARM_BASE_PREL   ");
        break;
    case R_ARM_GOT_BREL:
        printf("R_ARM_GOT_BREL    ");
        break;
    case R_ARM_PLT32:
        printf("R_ARM_PLT32       ");
        break;
    case R_ARM_CALL:
        printf("R_ARM_CALL        ");
        break;
    case R_ARM_JUMP24:
        printf("R_ARM_JUMP24      ");
        break;
    case R_ARM_THM_JUMP24:
        printf("R_ARM_THM_JUMP24  ");
        break;
    case R_ARM_BASE_ABS:
        printf("R_ARM_BASE_ABS    ");
        break;
    case R_ARM_ALU_PCREL_7_0:
        printf("R_ARM_ALU_PCREL_7_0 ");
        break;
    case R_ARM_ALU_PCREL_15_8:
        printf("R_ARM_ALU_PCREL_15_8 ");
        break;
    case R_ARM_ALU_PCREL_23_15:
        printf("R_ARM_ALU_PCREL_23_15 ");
        break;
    case R_ARM_LDR_SBREL_11_0_NC:
        printf("R_ARM_LDR_SBREL_11_0_NC ");
        break;
    case R_ARM_ALU_SBREL_19_12_NC:
        printf("R_ARM_ALU_SBREL_19_12_NC ");
        break;
    case R_ARM_ALU_SBREL_27_20_CK:
        printf("R_ARM_ALU_SBREL_27_20_CK ");
        break;
    case R_ARM_TARGET1:
        printf("R_ARM_TARGET1     ");
        break;
    case R_ARM_SBREL31:
        printf("R_ARM_SBREL31     ");
        break;
    case R_ARM_V4BX:
        printf("R_ARM_V4BX        ");
        break;
    case R_ARM_TARGET2:
        printf("R_ARM_TARGET2     ");
        break;
    case R_ARM_PREL31:
        printf("R_ARM_PREL31      ");
        break;
    case R_ARM_MOVW_ABS_NC:
        printf("R_ARM_MOVW_ABS_NC ");
        break;
    case R_ARM_MOVT_ABS:
        printf("R_ARM_MOVT_ABS    ");
        break;
    case R_ARM_MOVW_PREL_NC:
        printf("R_ARM_MOVW_PREL_NC ");
        break;
    case R_ARM_MOVT_PREL:
        printf("R_ARM_MOVT_PREL   ");
        break;
    case R_ARM_THM_MOVW_ABS_NC:
        printf("R_ARM_THM_MOVW_ABS_NC ");
        break;
    case R_ARM_THM_MOVT_ABS:
        printf("R_ARM_THM_MOVT_ABS ");
        break;
    case R_ARM_THM_MOVW_PREL_NC:
        printf("R_ARM_THM_MOVW_PREL_NC ");
        break;
    case R_ARM_THM_MOVT_PREL:
        printf("R_ARM_THM_MOVT_PREL ");
        break;
    case R_ARM_THM_JUMP19:
        printf("R_ARM_THM_JUMP19  ");
        break;
    case R_ARM_THM_JUMP6:
        printf("R_ARM_THM_JUMP6   ");
        break;
    case R_ARM_THM_ALU_PREL_11_0:
        printf("R_ARM_THM_ALU_PREL_11_0 ");
        break;
    case R_ARM_THM_PC12:
        printf("R_ARM_THM_PC12    ");
        break;
    case R_ARM_ABS32_NOI:
        printf("R_ARM_ABS32_NOI   ");
        break;
    case R_ARM_REL32_NOI:
        printf("R_ARM_REL32_NOI   ");
        break;
    case R_ARM_ALU_PC_G0_NC:
        printf("R_ARM_ALU_PC_G0_NC ");
        break;
    case R_ARM_ALU_PC_G0:
        printf("R_ARM_ALU_PC_G0   ");
        break;
    case R_ARM_ALU_PC_G1_NC:
        printf("R_ARM_ALU_PC_G1_NC ");
        break;
    case R_ARM_ALU_PC_G1:
        printf("R_ARM_ALU_PC_G1   ");
        break;
    case R_ARM_ALU_PC_G2:
        printf("R_ARM_ALU_PC_G2   ");
        break;
    case R_ARM_LDR_PC_G1:
        printf("R_ARM_LDR_PC_G1   ");
        break;
    case R_ARM_LDR_PC_G2:
        printf("R_ARM_LDR_PC_G2   ");
        break;
    case R_ARM_LDRS_PC_G0:
        printf("R_ARM_LDRS_PC_G0  ");
        break;
    case R_ARM_LDRS_PC_G1:
        printf("R_ARM_LDRS_PC_G1  ");
        break;
    case R_ARM_LDRS_PC_G2:
        printf("R_ARM_LDRS_PC_G2  ");
        break;
    case R_ARM_LDC_PC_G0:
        printf("R_ARM_LDC_PC_G0   ");
        break;
    case R_ARM_LDC_PC_G1:
        printf("R_ARM_LDC_PC_G1   ");
        break;
    case R_ARM_LDC_PC_G2:
        printf("R_ARM_LDC_PC_G2   ");
        break;
    case R_ARM_ALU_SB_G0_NC:
        printf("R_ARM_ALU_SB_G0_NC ");
        break;
    case R_ARM_ALU_SB_G0:
        printf("R_ARM_ALU_SB_G0   ");
        break;
    case R_ARM_ALU_SB_G1_NC:
        printf("R_ARM_ALU_SB_G1_NC ");
        break;
    case R_ARM_ALU_SB_G1:
        printf("R_ARM_ALU_SB_G1   ");
        break;
    case R_ARM_ALU_SB_G2:
        printf("R_ARM_ALU_SB_G2   ");
        break;
    case R_ARM_LDR_SB_G0:
        printf("R_ARM_LDR_SB_G0   ");
        break;
    case R_ARM_LDR_SB_G1:
        printf("R_ARM_LDR_SB_G1   ");
        break;
    case R_ARM_LDR_SB_G2:
        printf("R_ARM_LDR_SB_G2   ");
        break;
    case R_ARM_LDRS_SB_G0:
        printf("R_ARM_LDRS_SB_G0  ");
        break;
    case R_ARM_LDRS_SB_G1:
        printf("R_ARM_LDRS_SB_G1  ");
        break;
    case R_ARM_LDRS_SB_G2:
        printf("R_ARM_LDRS_SB_G2  ");
        break;
    case R_ARM_LDC_SB_G0:
        printf("R_ARM_LDC_SB_G0   ");
        break;
    case R_ARM_LDC_SB_G1:
        printf("R_ARM_LDC_SB_G1   ");
        break;
    case R_ARM_LDC_SB_G2:
        printf("R_ARM_LDC_SB_G2   ");
        break;
    case R_ARM_MOVW_BREL_NC:
        printf("R_ARM_MOVW_BREL_NC ");
        break;
    case R_ARM_MOVT_BREL:
        printf("R_ARM_MOVT_BREL   ");
        break;
    case R_ARM_MOVW_BREL:
        printf("R_ARM_MOVW_BREL   ");
        break;
    case R_ARM_THM_MOVW_BREL_NC:
        printf("R_ARM_THM_MOVW_BREL_NC ");
        break;
    case R_ARM_THM_MOVT_BREL:
        printf("R_ARM_THM_MOVT_BREL ");
        break;
    case R_ARM_THM_MOVW_BREL:
        printf("R_ARM_THM_MOVW_BREL ");
        break;
    case R_ARM_TLS_GOTDESC:
        printf("R_ARM_TLS_GOTDESC ");
        break;
    case R_ARM_TLS_CALL:
        printf("R_ARM_TLS_CALL    ");
        break;
    case R_ARM_TLS_DESCSEQ:
        printf("R_ARM_TLS_DESCSEQ ");
        break;
    case R_ARM_THM_TLS_CALL:
        printf("R_ARM_THM_TLS_CALL ");
        break;
    case R_ARM_PLT32_ABS:
        printf("R_ARM_PLT32_ABS   ");
        break;
    case R_ARM_GOT_ABS:
        printf("R_ARM_GOT_ABS     ");
        break;
    case R_ARM_GOT_PREL:
        printf("R_ARM_GOT_PREL    ");
        break;
    case R_ARM_GOT_BREL12:
        printf("R_ARM_GOT_BREL12  ");
        break;
    case R_ARM_GOTOFF12:
        printf("R_ARM_GOTOFF12    ");
        break;
    case R_ARM_GOTRELAX:
        printf("R_ARM_GOTRELAX    ");
        break;
    case R_ARM_GNU_VTENTRY:
        printf("R_ARM_GNU_VTENTRY ");
        break;
    case R_ARM_GNU_VTINHERIT:
        printf("R_ARM_GNU_VTINHERIT ");
        break;
    case R_ARM_THM_JUMP11:
        printf("R_ARM_THM_JUMP11  ");
        break;
    case R_ARM_THM_JUMP8:
        printf("R_ARM_THM_JUMP8   ");
        break;
    case R_ARM_TLS_GD32:
        printf("R_ARM_TLS_GD32    ");
        break;
    case R_ARM_TLS_LDM32:
        printf("R_ARM_TLS_LDM32   ");
        break;
    case R_ARM_TLS_LDO32:
        printf("R_ARM_TLS_LDO32   ");
        break;
    case R_ARM_TLS_IE32:
        printf("R_ARM_TLS_IE32    ");
        break;
    case R_ARM_TLS_LE32:
        printf("R_ARM_TLS_LE32    ");
        break;
    case R_ARM_TLS_LDO12:
        printf("R_ARM_TLS_LDO12   ");
        break;
    case R_ARM_TLS_LE12:
        printf("R_ARM_TLS_LE12    ");
        break;
    case R_ARM_TLS_IE12GP:
        printf("R_ARM_TLS_IE12GP  ");
        break;
    case R_ARM_PRIVATE_0:
        printf("R_ARM_PRIVATE_0   ");
        break;
    case R_ARM_PRIVATE_1:
        printf("R_ARM_PRIVATE_1   ");
        break;
    case R_ARM_PRIVATE_2:
        printf("R_ARM_PRIVATE_2   ");
        break;
    case R_ARM_PRIVATE_3:
        printf("R_ARM_PRIVATE_3   ");
        break;
    case R_ARM_PRIVATE_4:
        printf("R_ARM_PRIVATE_4   ");
        break;
    case R_ARM_PRIVATE_5:
        printf("R_ARM_PRIVATE_5   ");
        break;
    case R_ARM_PRIVATE_6:
        printf("R_ARM_PRIVATE_6   ");
        break;
    case R_ARM_PRIVATE_7:
        printf("R_ARM_PRIVATE_7   ");
        break;
    case R_ARM_PRIVATE_8:
        printf("R_ARM_PRIVATE_8   ");
        break;
    case R_ARM_PRIVATE_9:
        printf("R_ARM_PRIVATE_9   ");
        break;
    case R_ARM_PRIVATE_10:
        printf("R_ARM_PRIVATE_10  ");
        break;
    case R_ARM_PRIVATE_11:
        printf("R_ARM_PRIVATE_11  ");
        break;
    case R_ARM_PRIVATE_12:
        printf("R_ARM_PRIVATE_12  ");
        break;
    case R_ARM_PRIVATE_13:
        printf("R_ARM_PRIVATE_13  ");
        break;
    case R_ARM_PRIVATE_14:
        printf("R_ARM_PRIVATE_14  ");
        break;
    case R_ARM_PRIVATE_15:
        printf("R_ARM_PRIVATE_15  ");
        break;
    case R_ARM_ME_TOO:
        printf("R_ARM_ME_TOO      ");
        break;
    case R_ARM_THM_TLS_DESCSEQ16:
        printf("R_ARM_THM_TLS_DESCSEQ16 ");
        break;
    case R_ARM_THM_TLS_DESCSEQ32:
        printf("R_ARM_THM_TLS_DESCSEQ32 ");
        break;
    case R_ARM_THM_GOT_BREL12:
        printf("R_ARM_THM_GOT_BREL12 ");
        break;
    case R_ARM_THM_ALU_ABS_G0_NC:
        printf("R_ARM_THM_ALU_ABS_G0_NC ");
        break;
    case R_ARM_THM_ALU_ABS_G1_NC:
        printf("R_ARM_THM_ALU_ABS_G1_NC ");
        break;
    case R_ARM_THM_ALU_ABS_G2_NC:
        printf("R_ARM_THM_ALU_ABS_G2_NC ");
        break;
    case R_ARM_THM_ALU_ABS_G3:
        printf("R_ARM_THM_ALU_ABS_G3 ");
        break;
    case R_ARM_IRELATIVE:
        printf("R_ARM_IRELATIVE   ");
        break;
    default:
        printf("                  ");
        break;
    }
}
