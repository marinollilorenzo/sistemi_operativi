#!/bin/bash

if (( $# != 2 ))
then
    echo "Errore: devi passare due argomenti"
else
    if [[ -f $1 && -f $2 ]]
    then
        echo "Questo è il contenuto di $1:"
        cat $1
        echo "Questo è il contenuto di $2:"
        cat $2
    else
        echo "Almeno uno dei due path non è un file"
    fi
fi
