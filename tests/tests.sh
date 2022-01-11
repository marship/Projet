#!/bin/bash

# On récupère le chemin absolu de la racine du projet
PROJET="$(realpath $0)"
PROJET="${PROJET%/*/*}"

source ${PROJET}/tests/utils/couleurs.sh
source ${PROJET}/tests/utils/fonctions.sh
source ${PROJET}/tests/utils/variables.sh

# --------------------------------------------------------------------------------

# On vérifie qu'il y a bien un et un seul argument
if [ $# -ne 1 ]
then
    usage
    exit 1
fi

recuperer_option $1

# --------------------------------------------------------------------------------

source ${PROJET}/tests/tests_main.sh

afficher_separateur

# --------------------------------------------------------------------------------

# source ${PROJET}/tests/tests_elf_header.sh

options=("-h" "-S" "-s" "-r" "-x=1" "-x=.strtab" "-a")

for opt in "${options[@]}"
do
    if [ ${opt} = "-h" ]
    then
        CATEGORIE="Tests d'affichage de l'en-tête de fichier ELF"
    elif [ ${opt} = "-S" ]
    then
        CATEGORIE="Tests d'affichage de l'en-tête des sections"
    elif [ ${opt} = "-s" ]
    then
        CATEGORIE="Tests d'affichage de la table des symboles"
    elif [ ${opt} = "-r" ]
    then
        CATEGORIE="Tests d'affichage des réadressages"
    elif [ ${opt} = "-x=1" ]
    then
        CATEGORIE="Tests d'affichage du contenu de la section 1"
    elif [ ${opt} = "-x=.strtab" ]
    then
        CATEGORIE="Tests d'affichage du contenu de la section .strtab"
    elif [ ${opt} = "-a" ]
    then
        CATEGORIE="Tests d'affichage complet"
    fi

    afficher_categorie

    # ----------------------------------------

    TESTS="tests/erreurs"

    for file in ${PROJET}/${TESTS}/*
    do
        # On récupère l'affichage de notre programme
        OBTENU_STDOUT=$(${PROJET}/main ${opt} ${file} 2>/dev/null)
        OBTENU_STDERR=$(${PROJET}/main ${opt} ${file} 2>&1 >/dev/null)
        IFS=$'\n' read -d '' -a OBTENU_STDERR <<< "$OBTENU_STDERR"

        # On stocke l'affichage que l'on souhaite obtenir
        ATTENDU_STDOUT=$(arm-none-eabi-readelf ${opt} ${file} 2>/dev/null)
        ATTENDU_STDERR=$(grep '^readelf:\ ' <(readelf ${opt} ${file} 2>&1 >/dev/null))
        ATTENDU_STDERR="${ATTENDU_STDERR//readelf: /}"
        IFS=$'\n' read -d '' -a ATTENDU_STDERR <<< "$ATTENDU_STDERR"

        # On regarde si le test réussi ou non    
        i=0
        DIFF=0
        ERREUR=0

        while [ $i -lt ${#ATTENDU_STDERR[@]} ] && [ $(echo "${ATTENDU_STDERR[$i]}" | awk '{print $1;}') = "AVERTISSEMENT:" ]
        do
            diff <(echo "${OBTENU_STDERR[$i]}") <(echo "${ATTENDU_STDERR[$i]}") &>/dev/null
            DIFF=$(($DIFF+$?))
            i=$(($i+1))
        done

        if [ $DIFF -eq 0 ] && [ $i -lt ${#ATTENDU_STDERR[@]} ]
        then
            diff <(echo "${OBTENU_STDERR[$i]}") <(echo "${ATTENDU_STDERR[$i]}") &>/dev/null
            DIFF=$?
            i=$(($i+1))
            ERREUR=1
        fi

        if [ $ERREUR -eq 0 ]
        then
            diff <(echo "${OBTENU_STDOUT}") <(echo "${ATTENDU_STDOUT}") &>/dev/null
            DIFF=$(($DIFF+$?))
        fi


        # On affiche en fonction de l'option donnée en argument
        if [ $AFFICHER -eq 2 ]
        then
            printf "${CYAN}${TESTS}/${file##*/} : ${RESET}"

            # Si le test a réussi
            if [ $DIFF -eq 0 ]
            then
                printf "${GREEN}PASSED${RESET}\n\n"

                if [ $ERREUR -eq 0 ]
                then
                    echo "${OBTENU_STDOUT}"
                fi

                for j in $(seq 0 $i)
                do
                    echo ${OBTENU_STDERR[$j]}
                done

            # Si le test a échoué
            else
                printf "${RED}FAILED${RESET}\n\n"

                if [ $ERREUR -eq 0 ]
                then
                    diff <(echo "${OBTENU_STDOUT}") <(echo "${ATTENDU_STDOUT}")
                fi
                diff <(echo "${OBTENU_STDERR[@]}") <(echo "${ATTENDU_STDERR[@]}")
                printf "\n"

            fi

        elif [ $AFFICHER -eq 1 ]
        then
            # Si le test a échoué
            if [ $DIFF -eq 1 ]
            then
                if [ $CATEGORIE_AFFICHEE -eq 0 ]
                then
                    printf "${YELLOW}${CATEGORIE}${RESET}\n\n"
                    CATEGORIE_AFFICHEE=1
                fi

                printf "${CYAN}${TESTS}/${file##*/} : ${RESET}"
                printf "${RED}FAILED${RESET}\n\n"

                if [ $ERREUR -eq 0 ]
                then
                    diff <(echo "${OBTENU_STDOUT}") <(echo "${ATTENDU_STDOUT}")
                fi
                diff <(echo "${OBTENU_STDERR[@]}") <(echo "${ATTENDU_STDERR[@]}")

                printf "\n"
            fi

        else
            printf "${CYAN}${TESTS}/${file##*/} : ${RESET}"

            # Si le test a réussi
            if [ $DIFF -eq 0 ]
            then
                printf "${GREEN}PASSED${RESET}\n"
            else
                printf "${RED}FAILED${RESET}\n"
            fi

        fi
    done

    # ----------------------------------------

    TESTS="tests/corrects"

    for file in ${PROJET}/${TESTS}/*
    do
        # On récupère l'affichage de notre programme
        OBTENU_STDOUT=$(${PROJET}/main ${opt} ${file} 2>/dev/null)
        OBTENU_STDERR=$(${PROJET}/main ${opt} ${file} 2>&1 >/dev/null)

        # On stocke l'affichage que l'on souhaite obtenir
        ATTENDU_STDOUT=$(arm-none-eabi-readelf ${opt} ${file} 2>/dev/null)
        ATTENDU_STDERR=$(grep '^readelf:\ ' <(readelf ${opt} ${file} 2>&1 >/dev/null))

        # On regarde si le test réussi ou non    
        diff <(echo "${OBTENU_STDOUT}") <(echo "${ATTENDU_STDOUT}") &>/dev/null
        DIFF=$?
        diff <(echo "${OBTENU_STDERR}") <(echo "${ATTENDU_STDERR}") &>/dev/null
        DIFF=$(($DIFF+$?))

        afficher_test "${TESTS}/${file##*/}" "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" "${OBTENU_STDERR}" "${ATTENDU_STDERR}"
    done

    afficher_separateur
done
