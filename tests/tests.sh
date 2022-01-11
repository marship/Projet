#!/bin/bash

# On récupère le chemin absolu de la racine du projet
PROJET="$(realpath $0)"
PROJET="${PROJET%/*/*}"

source ${PROJET}/tests/utils/couleurs.sh
source ${PROJET}/tests/utils/fonctions.sh
source ${PROJET}/tests/utils/variables.sh

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
         [ $arg = "-a" ] || [ $arg = "--all" ] || \
         [ $arg = "-h" ] || [ $arg = "--file-header" ] || \
         [ $arg = "-S" ] || [ $arg = "--section-headers" ] || \
         [ $arg = "-s" ] || [ $arg = "--symbols" ] || \
         [ $arg = "-r" ] || [ $arg = "--relocs" ]
    then
        options+=("$arg")

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
    options=("-m" "-h" "-S" "-s" "-r" "--hex-dump=1" "--hex-dump=.strtab" "-a")
fi

# --------------------------------------------------------------------------------

for opt in "${options[@]}"
do
    if [[ $opt = "-m" || $opt = "--main" ]]
    then
        CATEGORIE="Tests du main"
        afficher_categorie
        effectuer_tests_main
    else
        if [[ $opt = "-a" || $opt = "--all" ]]
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
        effectuer_tests
    fi

    afficher_separateur
done
