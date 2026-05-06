#!/bin/bash

echo "scrivimi le stringhe separate dallo spazio"
read s1 s2

if [[ $s1 < $s2 ]]
then
	echo "$s1 viene prima di $s2"
elif [[ $s1 > $s2 ]]
then 
	echo "$s1 viene dopo di $s2"
else
	echo "$s1 è uguale ad $s2"
fi
