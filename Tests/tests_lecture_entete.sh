#!/bin/bash

CHEMIN="$(realpath $0)"
CHEMIN="${CHEMIN%/*}"
CHEMIN="${CHEMIN%/*}"

GREEN='\033[0;32m'
CYAN='\033[0;36m'
RESET='\033[0m'

printf "${GREEN}Tests_lecture_entete${RESET}\n\n"

for file in $CHEMIN/Tests/Tests_lecture_entete/*
do
    printf "${CYAN}${file##*/}${RESET}\n"
    diff <($CHEMIN/main "$file") <(arm-none-eabi-readelf -h "$file")
    printf "\n"
done
