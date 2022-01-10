#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include "elf_header.h"
#include "section_header.h"
#include "string_table.h"


void usage(char *name)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s <options> fichiers-elf\n"
            "\n"
            "Options:\n"
            "  -h --file-header       Afficher l'en-tête du fichier ELF\n"
            "  -S --section-headers   Afficher les en-têtes des sections\n"
            "  -s --symbols           Afficher la table des symboles\n"
            "  -r --relocs            Afficher les réadressages (si présents)\n"
            "  -H --help              Afficher l'aide-mémoire\n",
            name);
}

int main(int argc, char **argv)
{
    int opt;
    int opt_h = 0, opt_S = 0, opt_s = 0, opt_r = 0;

    if (argc < 2)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    struct option longopts[] = {
        { "file-header", no_argument, NULL, 'h' },
        { "section-headers", no_argument, NULL, 'S' },
        { "symbols", no_argument, NULL, 's' },
        { "relocs", no_argument, NULL, 'r' },
        { "help", no_argument, NULL, 'H' },
        { NULL, 0, NULL, 0 }
    };

    while ((opt = getopt_long(argc, argv, "hSsrH", longopts, NULL)) != -1) {
        switch(opt) {
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
        case 'H':
            usage(argv[0]);
            return EXIT_SUCCESS;
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            char *nom_fichier = argv[i];

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
            // char *strtab = lire_strtab(shdr, ehdr, nom_fichier);
            char *shstrtab = lire_shstrtab(f, shdr, ehdr);
            // Elf32_Sym *sym = lire_symbole(nom_fichier, ehdr, shdr);
            // Relocations *reloc = lire_relocations(nom_fichier, ehdr, shdr);

            fclose(f);

            if (opt_h) {
                afficher_elf_header(ehdr);
            }

            if (opt_S) {
                if (!opt_h) {
                    printf("There are %d section headers, starting at offset 0x%x:\n", ehdr.e_shnum, ehdr.e_shoff);
                }
                afficher_section_header(shdr, ehdr, shstrtab);
            }

            if (opt_s) {
                // afficher_symboles(sym, nom_fichier);
            }

            if (opt_r) {
                // afficher_relocations(reloc, ehdr, shdr, shstrtab, sym);
            }
        }
    }

    return EXIT_SUCCESS;
}
