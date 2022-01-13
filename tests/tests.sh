#!/bin/bash

# On récupère le chemin absolu de la racine du projet
PROJET="$(realpath $0)"
PROJET="${PROJET%/*/*}"

source "${PROJET}/tests/utils/couleurs.sh"
source "${PROJET}/tests/utils/fonctions.sh"
source "${PROJET}/tests/utils/variables.sh"

# --------------------------------------------------------------------------------

# On récupère les arguments

AFFICHER=0
options=()

for (( i = 1; i <= $#; i++ ))
do
    arg=${!i}

    if [[ $arg = "all" && $AFFICHER -eq 0 ]]
    then
        AFFICHER=3

    elif [[ $arg = "failed" && $AFFICHER -eq 0 ]]
    then
        AFFICHER=2

    elif [[ $arg = "minimal" && $AFFICHER -eq 0 ]]
    then
        AFFICHER=1

    elif [[ $arg = "all" || $arg = "failed" || $arg = "minimal" ]]
    then
        usage
        exit 1

    elif [ $arg = "help" ]
    then
        usage
        exit 0

    elif [ $arg = "-m" ] || [ $arg = "--main" ] || \
         [ $arg = "-h" ] || [ $arg = "--file-header" ] || \
         [ $arg = "-S" ] || [ $arg = "--section-headers" ] || \
         [ $arg = "-s" ] || [ $arg = "--symbols" ] || \
         [ $arg = "-r" ] || [ $arg = "--relocs" ]
    then
        options+=("$arg")

    elif [[ $arg = "-a" || $arg = "--all" ]]
    then
        options+=("-Sshr")

    elif [[ $arg = "-x" || $arg = "--hex-dump" ]]
    then
        i=$(($i+1))

        if [ $i -gt $# ]
        then
            usage
            exit 1
        else
            options+=("--hex-dump=${!i}")
        fi

    elif [ "${arg:0:3}" = "-x=" ]
    then
        options+=("--hex-dump=${arg:3}")

    elif [ "${arg:0:11}" = "--hex-dump=" ]
    then
        options+=("--hex-dump=${arg:11}")

    else
        usage
        exit 1
    fi
done

# On vérifie q'une commande a bien été donnée
if [ $AFFICHER -eq 0 ]
then
    usage
    exit 1
fi

# --------------------------------------------------------------------------------

if [ $# -eq 0 ]
then
    usage
    exit 1
elif [ $# -eq 1 ]
then
    options=("-m" "-h" "-S" "-s" "-r" "-Sshr" "--hex-dump=1" "--hex-dump=2" "--hex-dump=3" \
             "--hex-dump=4" "--hex-dump=5" "--hex-dump=.debug_aranges" "--hex-dump=.rel.debug_aranges" \
             "--hex-dump=.symtab" "--hex-dump=.shstrtab" "--hex-dump=.strtab")
fi

# --------------------------------------------------------------------------------

NB_TESTS=0
NB_PASSED=0

for opt in "${options[@]}"
do
    if [[ $opt = "-m" || $opt = "--main" ]]
    then
        CATEGORIE="Tests du main"
        afficher_categorie
        effectuer_tests_main
    else
        if [ $opt = "-Sshr" ]
        then
            CATEGORIE="Tests d'affichage complet"
        elif [[ $opt = "-h" || $opt = "--file-header" ]]
        then
            CATEGORIE="Tests d'affichage de l'en-tête de fichier ELF"
        elif [[ $opt = "-S" || $opt = "--section-headers" ]]
        then
            CATEGORIE="Tests d'affichage de l'en-tête des sections"
        elif [[ $opt = "-s" || $opt = "--symbols" ]]
        then
            CATEGORIE="Tests d'affichage de la table des symboles"
        elif [[ $opt = "-r" || $opt = "--relocs" ]]
        then
            CATEGORIE="Tests d'affichage des réadressages"
        elif [ "${opt:0:3}" = "-x=" ]
        then
            section=${opt:3}
            CATEGORIE="Tests d'affichage du contenu de la section ${section}"
        elif [ "${opt:0:11}" = "--hex-dump=" ]
        then
            section=${opt:11}
            CATEGORIE="Tests d'affichage du contenu de la section ${section}"
        fi

        afficher_categorie

        for file in ${PROJET}/tests/fichiers/*
        do
            effectuer_tests
        done

        file="${PROJET}/tests/fichiers/example*"
        effectuer_tests

        file="${PROJET}/tests/fichiers/test*"
        effectuer_tests
    fi

    afficher_separateur
done


NB_FAILED=$((NB_TESTS - NB_PASSED))
TAUX_PASSED=$((NB_PASSED * 100 / NB_TESTS))
TAUX_FAILED=$((NB_FAILED * 100 / NB_TESTS))

printf "Tests réussis : %3d sur %3d (%d%%)\n" ${NB_PASSED} ${NB_TESTS} ${TAUX_PASSED}
printf "Tests échoués : %3d sur %3d (%d%%)\n" ${NB_FAILED} ${NB_TESTS} ${TAUX_FAILED}
