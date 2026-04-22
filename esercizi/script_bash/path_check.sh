#!/bin/bash

echo "inserisci il primo path"
read path1
echo "inserisci il secondo path"
read path2

if [[ $path1 = $path2 ]]
then
    if [[ -d $path1 ]]
    then
        echo "Il path $path1 è una cartella"
    else
        echo "il path $path1 non è una cartella"
    fi
else
    echo "I due path sono diversi"
fi
