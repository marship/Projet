#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lecture_entete.h"
#include "entete_elf.h"


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Nombre d'arguments incorrect\n");
        return EXIT_FAILURE;
    }

    lire_entete(argv[1]);

    return EXIT_SUCCESS;
}
