#!/bin/bash


usage() {
    echo "Usage:"
    echo "  $0 <command>            Lancer tous les tests"
    echo "  $0 <command> <options>  Lancer uniquement les tests correspondant aux options données"
    echo ""
    echo "Commands:"
    echo "  all        Activer tous les affichages"
    echo "  failed     Afficher uniquement les tests qui échouent"
    echo "  minimal    Afficher uniquement si les tests réussissent ou échouent"
    echo "  help       Afficher ce message d'aide"
    echo ""
    echo "Options:"
    echo "  -m --main                  Tester le main"
    echo "  -a --all                   Équivalent à: -h -S -s -r"
    echo "  -h --file-header           Tester l'affichage de l'en-tête du fichier ELF"
    echo "  -S --section-headers       Tester l'affichage des en-têtes des sections"
    echo "  -s --symbols               Tester l'affichage de la table des symboles"
    echo "  -r --relocs                Tester l'affichage des réadressages"
    echo "  -x --hex-dump=<numéro|nom> Tester l'affichage du contenu de la section <numéro|nom>"
}


# $1 = test à afficher
# $2 = DIFF
# $3 = OBTENU_1
# $4 = ATTENDU_1
# $5 = OBTENU_2
# $6 = ATTENDU_2
afficher_test() {
    # On affiche en fonction de l'option donnée en argument
    if [ $AFFICHER -eq 3 ]
    then
        printf "${CYAN}$1 : ${RESET}"

        # Si le test a réussi
        if [ $2 -eq 0 ]
        then
            printf "${GREEN}PASSED${RESET}\n\n"
            
            if [ ! -z "$3" ]
            then
                echo "$3"
            fi
            if [ ! -z "$5" ]
            then
                echo "$5"
            fi

        # Sinon le test a échoué
        else
            printf "${RED}FAILED${RESET}\n\n"

            diff <(echo "$3") <(echo "$4")
            diff <(echo "$5") <(echo "$6")
        fi

        printf "\n"

    elif [ $AFFICHER -eq 2 ]
    then
        # Si le test a échoué
        if [ $2 -eq 1 ]
        then
            if [ $CATEGORIE_AFFICHEE -eq 0 ]
            then
                printf "${YELLOW}${CATEGORIE}${RESET}\n\n"
                CATEGORIE_AFFICHEE=1
            fi

            printf "${CYAN}$1 : ${RESET}"
            printf "${RED}FAILED${RESET}\n\n"

            diff <(echo "$3") <(echo "$4")
            diff <(echo "$5") <(echo "$6")

            printf "\n"
        fi

    else
        printf "${CYAN}$1 : ${RESET}"

        # Si le test a réussi
        if [ $2 -eq 0 ]
        then
            printf "${GREEN}PASSED${RESET}\n"
        # Sinon le test a échoué
        else
            printf "${RED}FAILED${RESET}\n"
        fi

    fi
}


afficher_categorie() {
    if [ $AFFICHER -eq 2 ]
    then
        CATEGORIE_AFFICHEE=0
    else
        printf "${YELLOW}${CATEGORIE}${RESET}\n\n"
        CATEGORIE_AFFICHEE=1
    fi
}


afficher_separateur() {
    if [ $AFFICHER -eq 1 ]
    then
        printf "\n"
    fi

    if [ $CATEGORIE_AFFICHEE -eq 1 ]
    then
        printf "${BLACK}--------------------------------------------------${RESET}\n\n"
    fi
}


effectuer_tests() {
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
        afficher_test "${file##*/}" 0 "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" \
                      "${OBTENU_STDERR}" "${ATTENDU_STDERR[@]}"
        NB_PASSED=$((NB_PASSED + 1))

    # Si les stderr sont identiques mais pas les stdout
    elif [ $DIFF_ERR -eq 0 ]
    then
        # Si les stderr sont vides
        if [ -z "${OBTENU_STDERR}" ]
        then
            afficher_test "${file##*/}" 1 "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" \
                          "${OBTENU_STDERR}" "${ATTENDU_STDERR[@]}"
        # Sinon les stderr ne sont pas vides
        else
            afficher_test "${file##*/}" 0 "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" \
                          "${OBTENU_STDERR}" "${ATTENDU_STDERR[@]}"
            NB_PASSED=$((NB_PASSED + 1))
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

        afficher_test "${file##*/}" $DIFF_ERR "${OBTENU_STDOUT}" "${ATTENDU_STDOUT}" \
                      "${OBTENU_STDERR}" "${ATTENDU_STDERR[@]}"
        if [ $DIFF_ERR -eq 0 ]
        then
            NB_PASSED=$((NB_PASSED + 1))
        fi

    fi

    NB_TESTS=$((NB_TESTS + 1))
}


effectuer_tests_main() {
    OBTENU=$(${PROJET}/main 2>&1)
    diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
    DIFF=$?
    afficher_test "test aucun argument" $DIFF "${OBTENU}" "${USAGE_MAIN}"

    if [ $DIFF -eq 0 ]
    then
        NB_PASSED=$((NB_PASSED + 1))
    fi

    OBTENU=$(${PROJET}/main ${PROJET}/tests/corrects/example1 2>&1)
    diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
    DIFF=$?
    afficher_test "test aucune option" $DIFF "${OBTENU}" "${USAGE_MAIN}"

    if [ $DIFF -eq 0 ]
    then
        NB_PASSED=$((NB_PASSED + 1))
    fi

    OBTENU=$(${PROJET}/main -a 2>&1)
    diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
    DIFF=$?
    afficher_test "test aucun fichier" $DIFF "${OBTENU}" "${USAGE_MAIN}"

    if [ $DIFF -eq 0 ]
    then
        NB_PASSED=$((NB_PASSED + 1))
    fi

    OBTENU=$(${PROJET}/main -H 2>&1)
    diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
    DIFF=$?
    afficher_test "test affichage aide" $DIFF "${OBTENU}" "${USAGE_MAIN}"

    if [ $DIFF -eq 0 ]
    then
        NB_PASSED=$((NB_PASSED + 1))
    fi

    OBTENU=$(${PROJET}/main -q 2>&1)
    diff <(echo "${OBTENU}") <(echo "${OPTION_INVALIDE}${USAGE_MAIN}") &>/dev/null
    DIFF=$?
    afficher_test "test option invalide" $DIFF "${OBTENU}" "${OPTION_INVALIDE}${USAGE_MAIN}"

    if [ $DIFF -eq 0 ]
    then
        NB_PASSED=$((NB_PASSED + 1))
    fi

    OBTENU=$(${PROJET}/main -a aaa 2>&1)
    diff <(echo "${OBTENU}") <(echo "${FICHIER_INEXISTANT}") &>/dev/null
    DIFF=$?
    afficher_test "test fichier inexistant" $DIFF "${OBTENU}" "${FICHIER_INEXISTANT}"

    if [ $DIFF -eq 0 ]
    then
        NB_PASSED=$((NB_PASSED + 1))
    fi

    OBTENU=$(${PROJET}/main ${PROJET}/tests/corrects/example1 -x 2>&1)
    diff <(echo "${OBTENU}") <(echo "${OPTION_SANS_ARGUMENT}${USAGE_MAIN}") &>/dev/null
    DIFF=$?
    afficher_test "test option -x sans argument" $DIFF "${OBTENU}" "${OPTION_SANS_ARGUMENT}${USAGE_MAIN}"

    if [ $DIFF -eq 0 ]
    then
        NB_PASSED=$((NB_PASSED + 1))
    fi

    NB_TESTS=$((NB_TESTS + 7))
}
