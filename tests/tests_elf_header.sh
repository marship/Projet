#!/bin/bash

# On récupère le chemin absolu de la racine du projet
PROJET="$(realpath $0)"
PROJET="${PROJET%/*/*}"

source ${PROJET}/tests/utils/couleurs.sh
source ${PROJET}/tests/utils/fonctions.sh

# --------------------------------------------------------------------------------

# On vérifie qu'il y a bien un et un seul argument
if [ $# -ne 1 ]
then
    usage
    exit 1
fi

recuperer_option $1

# --------------------------------------------------------------------------------

CATEGORIE="Tests d'affichage de l'en-tête de fichier ELF"
afficher_categorie
opt="-h"


TESTS="tests/erreurs"

for file in ${PROJET}/${TESTS}/*
do
    # On récupère l'affichage de notre programme
    OBTENU_STDOUT=$(${PROJET}/main ${opt} ${file} 2>/dev/null)
    OBTENU_STDERR=$(${PROJET}/main ${opt} ${file} 2>&1 >/dev/null)

    # On stocke l'affichage que l'on souhaite obtenir
    ATTENDU_STDOUT=$(arm-none-eabi-readelf ${opt} ${file} 2>/dev/null)
    ATTENDU_STDERR=$(grep '^readelf:\ ' <(readelf ${opt} ${file} 2>&1 >/dev/null))
    ATTENDU_STDERR="${ATTENDU_STDERR//readelf: /}"
    IFS=$'\n' read -d '' -a ATTENDU_STDERR <<< "$ATTENDU_STDERR"

    # On regarde si les stdout et stderr sont identiques
    diff <(echo "${OBTENU_STDOUT}") <(echo "${ATTENDU_STDOUT}") &>/dev/null
    DIFF_OUT=$?
    diff <(echo "${OBTENU_STDERR}") <(echo "${ATTENDU_STDERR[@]}") &>/dev/null
    DIFF_ERR=$?

    # Si les stdout et stderr sont identiques
    if [ $DIFF_OUT -eq 0 ] && [ $DIFF_ERR -eq 0 ]
    then
        afficher_test "${TESTS}/${file##*/}" "$DIFF_OUT" "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" \
                      "${OBTENU_STDERR}" "${ATTENDU_STDERR[@]}"

    # Si les stderr sont identiques mais pas les stdout
    elif [ $DIFF_ERR -eq 0 ]
    then
        # Si les stderr sont vides
        if [ -z "${OBTENU_STDERR}" ]
        then
            afficher_test "${TESTS}/${file##*/}" "$DIFF_OUT" "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" \
                          "${OBTENU_STDERR}" "${ATTENDU_STDERR[@]}"
        # Sinon les stderr ne sont pas vides
        else
            afficher_test "${TESTS}/${file##*/}" "$DIFF_ERR" "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" \
                          "${OBTENU_STDERR}" "${ATTENDU_STDERR[@]}"
        fi

    # Sinon les stderr sont différentes
    else
        i=0

        # On compare chaque stderr attendue avec la stderr obtenue
        while [ $i -lt ${#ATTENDU_STDERR[@]} ] && [ $DIFF_ERR -ne 0 ]
        do
            diff <(echo "${OBTENU_STDERR}") <(echo "${ATTENDU_STDERR[$i]}") &>/dev/null
            DIFF_ERR=$?
            i=$(($i+1))
        done

        afficher_test "${TESTS}/${file##*/}" "$DIFF_ERR" "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" \
                      "${OBTENU_STDERR}" "${ATTENDU_STDERR[@]}"

    fi
done
