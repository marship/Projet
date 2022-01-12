#!/bin/bash


USAGE_MAIN=$(cat <<-END
Usage:
  ${PROJET}/main <options> fichiers-elf

Options:
  -a --all                   Équivalent à: -h -S -s -r
  -h --file-header           Afficher l'en-tête du fichier ELF
  -S --section-headers       Afficher les en-têtes des sections
  -s --symbols               Afficher la table des symboles
  -r --relocs                Afficher les réadressages (si présents)
  -x --hex-dump=<numéro|nom> Afficher le contenu de la section <numéro|nom> sous forme d'octets
  -p --partie                
  -H --help                  Afficher l'aide-mémoire
END
)

OPTION_INVALIDE="${PROJET}/main: invalid option -- 'q'"$'\n'

FICHIER_INEXISTANT="ERREUR: Impossible d'ouvir le fichier : aaa"

OPTION_SANS_ARGUMENT="${PROJET}/main: option requires an argument -- 'x'"$'\n'
