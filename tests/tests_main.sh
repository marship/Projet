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

CATEGORIE="Tests du main"
afficher_categorie


OBTENU=$(${PROJET}/main 2>&1)
diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
afficher_test "test aucun argument" "$?" "${OBTENU}" "${USAGE_MAIN}"


OBTENU=$(${PROJET}/main ${PROJET}/tests/corrects/example1 2>&1)
diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
afficher_test "test aucune option" "$?" "${OBTENU}" "${USAGE_MAIN}"


OBTENU=$(${PROJET}/main -a 2>&1)
diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
afficher_test "test aucun fichier" "$?" "${OBTENU}" "${USAGE_MAIN}"


OBTENU=$(${PROJET}/main -H 2>&1)
diff <(echo "${OBTENU}") <(echo "${USAGE_MAIN}") &>/dev/null
afficher_test "test affichage aide" "$?" "${OBTENU}" "${USAGE_MAIN}"


OBTENU=$(${PROJET}/main -q 2>&1)
diff <(echo "${OBTENU}") <(echo "${OPTION_INVALIDE}${USAGE_MAIN}") &>/dev/null
afficher_test "test option invalide" "$?" "${OBTENU}" "${OPTION_INVALIDE}${USAGE_MAIN}"

OBTENU=$(${PROJET}/main -a aaa 2>&1)
diff <(echo "${OBTENU}") <(echo "${FICHIER_INEXISTANT}") &>/dev/null
afficher_test "test fichier inexistant" "$?" "${OBTENU}" "${FICHIER_INEXISTANT}"


OBTENU=$(${PROJET}/main ${PROJET}/tests/corrects/example1 -x 2>&1)
diff <(echo "${OBTENU}") <(echo "${OPTION_SANS_ARGUMENT}${USAGE_MAIN}") &>/dev/null
afficher_test "test option -x sans argument" "$?" "${OBTENU}" "${OPTION_SANS_ARGUMENT}${USAGE_MAIN}"
