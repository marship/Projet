#!/bin/bash

# On récupère le chemin absolu de la racine du projet
PROJET="$(realpath $0)"
PROJET="${PROJET%/*/*}"

# Variables pour colorer le texte
BLACK='\033[0;30m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
RESET='\033[0m'

# --------------------------------------------------------------------------------

# Fonctions

usage() {
    echo "Usage:"
    echo "  $0 <options>"
    echo ""
    echo "Options:"
    echo "  -a --all        Activer tous les affichages"
    echo "  -f --failed     Afficher uniquement les tests qui échouent"
    echo "  -m --minimal    Afficher uniquement si les tests réussissent ou échouent"
    echo "  -h --help       Afficher ce message d'aide"
}


# $1 = test à afficher
# $2 = $OBTENU_1
# $3 = $ATTENDU_1
# $4 = $OBTENU_2
# $5 = $ATTENDU_2
affichage() {
    # On affiche en fonction de l'option donnée en argument
    if [ $AFFICHER -eq 2 ]
    then
        printf "${CYAN}$1 : ${RESET}"

        # Si le test a réussi
        if [ $DIFF -eq 0 ]
        then
            printf "${GREEN}PASSED${RESET}\n\n"
            echo "$2"            

        # Sinon le test a échoué
        else
            printf "${RED}FAILED${RESET}\n\n"

            diff <(echo "$2") <(echo "$3")
            diff <(echo "$4") <(echo "$5")
        fi

        printf "\n"

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

            printf "${CYAN}$1 : ${RESET}"
            printf "${RED}FAILED${RESET}\n\n"

            diff <(echo "$2") <(echo "$3")
            diff <(echo "$4") <(echo "$5")

            printf "\n"
        fi

    else
        printf "${CYAN}$1 : ${RESET}"

        # Si le test a réussi
        if [ $DIFF -eq 0 ]
        then
            printf "${GREEN}PASSED${RESET}\n"
        # Sinon le test a échoué
        else
            printf "${RED}FAILED${RESET}\n"
        fi

    fi
}

afficher_categorie() {
    if [ $AFFICHER -eq 1 ]
    then
        CATEGORIE_AFFICHEE=0
    else
        printf "${YELLOW}${CATEGORIE}${RESET}\n\n"
        CATEGORIE_AFFICHEE=1
    fi
}

afficher_separateur() {
    if [ $AFFICHER -eq 0 ]
    then
        printf "\n"
    fi

    if [ $CATEGORIE_AFFICHEE -eq 1 ]
    then
        printf "${BLACK}--------------------------------------------------${RESET}\n\n"
    fi
}

# --------------------------------------------------------------------------------

# On vérifie qu'il y a bien un et un seul argument
if [ $# -ne 1 ]
then
    usage
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
    usage
    exit 0
else
    usage
    exit 1
fi

# --------------------------------------------------------------------------------

CATEGORIE="Tests du main"

afficher_categorie

# ----------------------------------------

USAGE_MAIN=$(cat <<-END
Usage:
  ${PROJET}/main <options> fichiers-elf

Options:
  -h --file-header           Afficher l'en-tête du fichier ELF
  -S --section-headers       Afficher les en-têtes des sections
  -s --symbols               Afficher la table des symboles
  -r --relocs                Afficher les réadressages (si présents)
  -x --hex-dump=<numéro|nom> Afficher le contenu de la section <numéro|nom> sous forme d'octets
  -H --help                  Afficher l'aide-mémoire
END
)

OBTENU=$(${PROJET}/main 2>&1)
diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
DIFF=$?

affichage "test aucun argument" "${OBTENU}" "${USAGE_MAIN}"

OBTENU=$(${PROJET}/main -H 2>&1)
diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
DIFF=$?

affichage "test affichage aide" "${OBTENU}" "${USAGE_MAIN}"

afficher_separateur

# --------------------------------------------------------------------------------

options=("-h" "-S" "-s" "-r" "-hSsr")

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
    elif [ ${opt} = "-hSsr" ]
    then
        CATEGORIE="Tests d'affichage complet"
    fi

    afficher_categorie

    # ----------------------------------------

ATTENDU=$(cat <<-END
${PROJET}/main: option requires an argument -- '$(echo ${opt:1:1})'
ERREUR: Impossible d'ouvir le fichier : (null)
END
)

    OBTENU=$(${PROJET}/main ${opt} 2>&1)
    diff <(echo "${OBTENU}") <(echo "${ATTENDU}") &>/dev/null
    DIFF=$?

    affichage "test aucune option" "${OBTENU}" "${ATTENDU}"

    # ----------------------------------------

ATTENDU=$(cat <<-END
ERREUR: Impossible d'ouvir le fichier : fichier
END
)

    OBTENU=$(${PROJET}/main ${opt} fichier 2>&1)
    diff <(echo "${OBTENU}") <(echo "${ATTENDU}") &>/dev/null
    DIFF=$?

    affichage "test fichier inexistant" "${OBTENU}" "${ATTENDU}"

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

        affichage "${TESTS}/${file##*/}" "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" "${OBTENU_STDERR}" "${ATTENDU_STDERR}"
    done

    afficher_separateur
done
