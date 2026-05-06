#!/bin/bash

if (( $# != 1 ))
then 
    echo "Errore nel passaggio degli argomenti"
else
    if(( $1 < 10 ))
    then
        for ((i = 0; i < 10; i++))
        do
            echo "creo file $i.txt"
            touch $i.txt
        done
    else
        echo "il numero $i non è minore di 10"
    fi
fi
