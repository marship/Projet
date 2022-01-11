#!/bin/bash


recuperer_option() {
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
}


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
# $2 = DIFF
# $3 = OBTENU_1
# $4 = ATTENDU_1
# $5 = OBTENU_2
# $6 = ATTENDU_2
afficher_test() {
    # On affiche en fonction de l'option donnée en argument
    if [ $AFFICHER -eq 2 ]
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

    elif [ $AFFICHER -eq 1 ]
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
