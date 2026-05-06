#!/bin/bash


while true
do
    echo "inserisci il path di un file; 'quit' per uscire"
    read path
    if [[ $path = "quit" ]]
    then
        break
    elif [[ ! -f $path ]]
    then
        echo "Errore:il path non è un file"
    else
        wc -l $path
    fi
done

