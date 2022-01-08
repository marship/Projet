#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include "lecture_entete.h"
#include "elf_header.h"
#include "fonctions_utilitaires.h"
#include "lecture_symbole.h"
#include "lecture_reimplantation.h"
#include "lecture_section.h"
#include "string_table.h"


void usage(char *name)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s <options> fichiers-elf\n"
            "\n"
            "Options:\n"
            "  -h --file-header       Afficher l'en-tête du fichier ELF\n"
			"  -S --section-header    Afficher l'en-tête des sections\n"
            "  -s --table-symbole     Afficher la table des symboles\n"
			"  -r --relocs            Afficher les réadressages (si présents)\n"
            "  -H --help              Afficher l'aide-mémoire\n",
            name);
}

int main(int argc, char **argv)
{

	int opt;
	char *nom_fichier = NULL;
	Elf32_Ehdr ehdr;
	Elf32_Shdr *shdr;
    char *strtab = NULL;
    char *shstrtab = NULL;
    Elf32_Sym *sym;
    Relocations *reloc;

    if (argc < 2)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

	struct option longopts[] = {
		{ "file-header", required_argument, NULL, 'h' },
        { "section-header ", required_argument, NULL, 'S' },
		{ "table-symbole", required_argument, NULL, 's' },
		{ "relocs", required_argument, NULL, 'r'},
		{ "help", no_argument, NULL, 'H' },
		{ NULL, 0, NULL, 0 }
	};


	while ((opt = getopt_long(argc, argv, "h:S:s:r:H", longopts, NULL)) != -1) {
		nom_fichier = optarg;
		ehdr = lire_entete(nom_fichier);
		shdr = lire_section(nom_fichier, ehdr);
        strtab = lire_strtab(shdr, ehdr, nom_fichier);
        shstrtab = lire_shstrtab(shdr, ehdr, nom_fichier);
        sym = lire_symbole(nom_fichier, ehdr, shdr);
        reloc = lire_relocations(nom_fichier, ehdr, shdr);

        switch(opt) {
		case 'h':
			afficher_entete(ehdr);
			break;
		case 'S':
			afficher_section(shdr, ehdr, shstrtab);
			break;
        case 's':
            afficher_symboles(sym, nom_fichier, strtab);
            break;
		case 'r':
            afficher_relocations(reloc, ehdr, shdr, shstrtab, sym);
			break;
		case 'H':
			usage(argv[0]);
			return EXIT_SUCCESS;
		default:
			usage(argv[0]);
			return EXIT_FAILURE;
		}
	}
    return EXIT_SUCCESS;
}
