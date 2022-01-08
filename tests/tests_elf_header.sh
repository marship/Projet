#!/bin/bash

# On récupère le chemin absolu de la racine du projet
PROJET="$(realpath $0)"
PROJET="${PROJET%/*/*}"

# Variables pour colorer le texte
BLACK='\033[0;30m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
RESET='\033[0m'

usage() {
    echo "Usage:"
    echo "  $1 <options>"
    echo ""
    echo "Options:"
    echo "  -a --all        Activer tous les affichages"
    echo "  -f --failed     Afficher uniquement les tests qui échouent"
    echo "  -m --minimal    Afficher uniquement si les tests réussissent ou échouent"
    echo "  -h --help       Afficher ce message d'aide"
}

# On vérifie qu'il y a bien un et un seul argument
if [ $# -ne 1 ]
then
    usage $0
    exit 1
fi

if [ $1 = "-a" ] || [ $1 = "--all" ]
then
    AFFICHER=2
elif [ $1 = "-f" ] || [ $1 = "--failed" ]
then
    AFFICHER=1
elif [ $1 = "-m" ] || [ $1 = "--minimal" ]
then
    AFFICHER=0
elif [ $1 = "-h" ] || [ $1 = "--help" ]
then
    usage $0
    exit 0
else
    usage $0
    exit 1
fi


TESTS="tests/erreurs"

for file in ${PROJET}/${TESTS}/*
do
    # On récupère l'affichage de notre programme
    OBTENU=$(${PROJET}/main -h ${file} 2>&1)

    # On stocke l'affichage que l'on souhaite obtenir
    # ATTENDU=$(readelf -h ${file} 2>&1)
    ATTENDU=$(grep '^readelf:\ ERREUR:\ ' <(readelf -h ${file} 2>&1))
    ATTENDU="${ATTENDU//readelf: /}"

    # On regarde si le test réussi ou non    
    diff <(echo "${OBTENU}") <(echo "${ATTENDU}") &>/dev/null
    DIFF=$?

    # On affiche en fonction de l'option donnée en argument
    if [ $AFFICHER -eq 2 ]
    then
        printf "${CYAN}${TESTS}/${file##*/} : ${RESET}"

        # Si le test a réussi
        if [ $DIFF -eq 0 ]
        then
            printf "${GREEN}PASSED${RESET}\n\n"
            echo "${OBTENU}"

        # Si le test a échoué
        else
            printf "${RED}FAILED${RESET}\n\n"
            diff <(echo "${OBTENU}") <(echo "${ATTENDU}")
        fi
        printf "\n${BLACK}--------------------------------------------------${RESET}\n\n"

    elif [ $AFFICHER -eq 1 ]
    then
        if [ $DIFF -eq 1 ]
        then
            printf "${CYAN}${TESTS}/${file##*/} : ${RESET}"

            printf "${RED}FAILED${RESET}\n\n"
            diff <(echo "${OBTENU}") <(echo "${ATTENDU}")
            printf "\n${BLACK}--------------------------------------------------${RESET}\n\n"
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


TESTS="tests/corrects"

for file in ${PROJET}/${TESTS}/*
do
    # On récupère l'affichage de notre programme
    OBTENU=$(${PROJET}/main -h ${file} 2>&1)

    # On stocke l'affichage que l'on souhaite obtenir
    ATTENDU=$(arm-none-eabi-readelf -h ${file} 2>&1)

    # On regarde si le test réussi ou non    
    diff <(echo "${OBTENU}") <(echo "${ATTENDU}") &>/dev/null
    DIFF=$?

    # On affiche en fonction de l'option donnée en argument
    if [ $AFFICHER -eq 2 ]
    then
        printf "${CYAN}${TESTS}/${file##*/} : ${RESET}"

        # Si le test a réussi
        if [ $DIFF -eq 0 ]
        then
            printf "${GREEN}PASSED${RESET}\n\n"
            echo "${OBTENU}"

        # Si le test a échoué
        else
            printf "${RED}FAILED${RESET}\n\n"
            diff <(echo "${OBTENU}") <(echo "${ATTENDU}")
        fi
        printf "\n${BLACK}--------------------------------------------------${RESET}\n\n"

    elif [ $AFFICHER -eq 1 ]
    then
        if [ $DIFF -eq 1 ]
        then
            printf "${CYAN}${TESTS}/${file##*/} : ${RESET}"

            printf "${RED}FAILED${RESET}\n\n"
            diff <(echo "${OBTENU}") <(echo "${ATTENDU}")
            printf "\n${BLACK}--------------------------------------------------${RESET}\n\n"
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
