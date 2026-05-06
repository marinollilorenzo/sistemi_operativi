#!/bin/bash

for file in *
do
    if [[ -f $file ]]
    then
        echo "'$file' è un file"
    elif [[ -d $file ]]
    then
        echo "'$file' è una directory"
    else
        echo "'$file' non è una directory e neanche un file"
    fi
done
