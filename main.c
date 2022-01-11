#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>
#include "elf_header.h"
#include "section_header.h"
#include "string_table.h"
#include "section.h"
#include "relocation.h"
#include "symbol.h"


void usage(char *name)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s <options> fichiers-elf\n"
            "\n"
            "Options:\n"
            "  -a --all                   Équivalent à: -h -S -s -r\n"
            "  -h --file-header           Afficher l'en-tête du fichier ELF\n"
            "  -S --section-headers       Afficher les en-têtes des sections\n"
            "  -s --symbols               Afficher la table des symboles\n"
            "  -r --relocs                Afficher les réadressages (si présents)\n"
            "  -x --hex-dump=<numéro|nom> Afficher le contenu de la section <numéro|nom> sous forme d'octets\n"
            "  -H --help                  Afficher l'aide-mémoire\n",
            name);
}

int main(int argc, char **argv)
{
    int opt;
    int opt_h = 0, opt_S = 0, opt_s = 0, opt_r = 0, opt_x = 0;
    int nb_fichiers = 0;

    if (argc < 2)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    char **arg_x = malloc((argc - 1) * sizeof(char *));

    if (arg_x == NULL) {
        fprintf(stderr, "ERREUR: Impossible d'allouer de la mémoire\n");
        return EXIT_FAILURE;
    }

    struct option longopts[] = {
        { "all", no_argument, NULL, 'a' },
        { "file-header", no_argument, NULL, 'h' },
        { "section-headers", no_argument, NULL, 'S' },
        { "symbols", no_argument, NULL, 's' },
        { "relocs", no_argument, NULL, 'r' },
        { "hex-dump", required_argument, NULL, 'x' },
        { "help", no_argument, NULL, 'H' },
        { NULL, 0, NULL, 0 }
    };

    while ((opt = getopt_long(argc, argv, "ahSsrx:H", longopts, NULL)) != -1) {
        switch (opt) {
        case 'a':
            opt_h = 1;
            opt_S = 1;
            opt_s = 1;
            opt_r = 1;
            break;

        case 'h':
            opt_h = 1;
            break;

        case 'S':
            opt_S = 1;
            break;

        case 's':
            opt_s = 1;
            break;

        case 'r':
            opt_r = 1;
            break;

        case 'x':
            opt_x++;

            arg_x[opt_x-1] = malloc(strlen(optarg) * sizeof(char));

            if (arg_x[opt_x-1] == NULL) {
                fprintf(stderr, "ERREUR: Impossible d'allouer de la mémoire\n");
                return EXIT_FAILURE;
            }

            strcpy(arg_x[opt_x-1], optarg);
            break;

        case 'H':
            usage(argv[0]);
            return EXIT_SUCCESS;

        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if ((!opt_h && !opt_S && !opt_s && !opt_r && !opt_x)) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' && strcmp(argv[i-1], "-x") != 0 && strcmp(argv[i-1], "--hex-dump") != 0) {
            char *nom_fichier = argv[i];
            nb_fichiers++;

            // Ouverture du fichier
            FILE *f = fopen(nom_fichier, "rb");

            if (f == NULL)
            {
                fprintf(stderr, "ERREUR: Impossible d'ouvir le fichier : %s\n", nom_fichier);
                exit(EXIT_FAILURE);
            }

            // On calcule la taille du fichier (en octets)
            fseek(f, 0L, SEEK_END);
            long taille = ftell(f);
    
            Elf32_Ehdr ehdr = lire_elf_header(f, nom_fichier);
            Elf32_Shdr *shdr = lire_section_header(f, ehdr, taille);
            char *strtab = lire_strtab(f, shdr, ehdr);
            char *shstrtab = lire_shstrtab(f, shdr, ehdr);
            Relocations *reloc = lire_relocations(f, ehdr, shdr);
            Elf32_Sym *sym = lire_symboles(f, ehdr, shdr);

            if (opt_h) {
                afficher_elf_header(ehdr);
            }

            if (opt_S) {
                if (!opt_h) {
                    if (ehdr.e_shnum == 1) {
                        printf("There is %d section header, starting at offset 0x%x:\n", ehdr.e_shnum, ehdr.e_shoff);
                    }
                    else if (ehdr.e_shnum > 1) {
                        printf("There are %d section headers, starting at offset 0x%x:\n", ehdr.e_shnum, ehdr.e_shoff);
                    }
                }
                afficher_section_header(shdr, ehdr, shstrtab);
            }

            if (opt_r) {
                afficher_relocations(reloc, ehdr, shdr, shstrtab, sym);
            }

            if (opt_s) {
                afficher_symboles(sym, ehdr, shdr, shstrtab, strtab);
            }

            if (opt_x) {
                long *num_x = malloc(opt_x * sizeof(long));

                if (num_x == NULL) {
                    fprintf(stderr, "ERREUR: Impossible d'allouer de la mémoire pour la section\n");
                    return EXIT_FAILURE;
                }

                for (int i = 0; i < opt_x; i++) {
                    char *end = NULL;
                    num_x[i] = strtol(arg_x[i], &end, 10);

                    if (num_x[i] == 0 && end == arg_x[i]) {
                        for (int j = 0; j < ehdr.e_shnum; j++) {
                            if (strcmp(arg_x[i], shstrtab+shdr[j].sh_name) == 0) {
                                num_x[i] = j;
                            }
                        }
                        if (num_x[i] == 0) {
                            fprintf(stderr, "AVERTISSEMENT: La section « %s » n'a pas été vidangée "
                                    "parce qu'inexistante !\n", arg_x[i]);
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if (num_x[i] >= ehdr.e_shnum) {
                        fprintf(stderr,
                                "AVERTISSEMENT: La section %ld n'a pas été vidangée parce qu'inexistante !\n",
                                num_x[i]);
                        exit(EXIT_FAILURE);
                    }
                }

                for (int i = 0; i < ehdr.e_shnum; i++) {
                    for (int j = 0; j < opt_x; j++) {
                        if (num_x[j] == i) {
                            unsigned char *section = lire_section(f, shdr, i);
                            afficher_section(section, shstrtab, shdr[i]);
                            free(section);
                        }
                    }
                }
                free(num_x);
            }

            if (shdr != NULL) {
                free(shdr);
            }
            if (strtab != NULL) {
                free(strtab);
            }
            if (shstrtab != NULL) {
                free(shstrtab);
            }
            liberer_relocations(reloc, ehdr);
            if (sym != NULL) {
                free(sym);
            }
            fclose(f);
        }
    }

    if (nb_fichiers == 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < opt_x; i++) {
        free(arg_x[i]);
    }
    free(arg_x);

    return EXIT_SUCCESS;
}
