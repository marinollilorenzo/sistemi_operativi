#!/bin/bash

echo "scrivi i due numeri separati da uno spazio: n1 n2"
read n1 n2
if ((n1 > n2)) 
then
	echo "$n1 è maggiore di $n2"
elif ((n1 < n2 )) 
then
	echo "$n1 è minore di $n2"
else
	echo "$n1 è uguale ad $n2"
fi
