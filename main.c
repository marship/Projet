#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include "lecture_entete.h"
#include "elf_header.h"
#include "fonctions_utilitaires.h"
#include "lecture_symbole.h"
#include "lecture_reimplantation.h"


void usage(char *name)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s <options> fichiers-elf\n"
            "\n"
            "Options:\n"
            "  -h --file-header       Afficher l'en-tête du fichier ELF\n"
            "  -s --table-symbole     Afficher la table des symboles\n"
			"  -r --relocs            Afficher les réadressages (si présents)\n"
            "  -H --help              Afficher l'aide-mémoire\n",
            name);
}

int main(int argc, char **argv)
{

	int opt;
	char *nom_fichier = NULL;

    if (argc < 2)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

	struct option longopts[] = {
		{ "file-header", required_argument, NULL, 'h' },
        { "table-symbole", required_argument, NULL, 's' },
		{ "relocs", required_argument, NULL, 'r'},
		{ "help", no_argument, NULL, 'H' },
		{ NULL, 0, NULL, 0 }
	};

	Elf32_Ehdr ehdr = lire_entete(nom_fichier);

	while ((opt = getopt_long(argc, argv, "h:s:H", longopts, NULL)) != -1) {
		switch(opt) {
		case 'h':
			nom_fichier = optarg;
			afficher_entete(ehdr);
			break;
        case 's':
			nom_fichier = optarg;
            lire_symbole(nom_fichier);
            break;
		case 'r':

			break;
		case 'H':
			usage(argv[0]);
			return EXIT_SUCCESS;
		default:
			usage(argv[0]);
			return EXIT_FAILURE;
		}
	}

    //     printf("\n\t\t\t-----MODE EMPLOI-----\n\n\th : Affichage de l'en-tete\n\tS : Affichage de la table des sections\n\tx : Affichage du contenu d'une section\n\ts : Affichage de la table des symboles\n\tr : Affichage des tables de reimplantation\n");

    return EXIT_SUCCESS;
}
