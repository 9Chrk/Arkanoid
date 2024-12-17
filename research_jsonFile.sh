#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <chemin_du_dossier> <prefixe_fichier>"
    exit 1
fi

CHEMIN_DOSSIER=$1
PREFIXE=$2

if [ ! -d "$CHEMIN_DOSSIER" ]; then
    echo "$CHEMIN_DOSSIER n'est pas un répertoire valide."
    exit 1
fi

find "$CHEMIN_DOSSIER" -type f -name "${PREFIXE}*.json" -printf "%f\n"
