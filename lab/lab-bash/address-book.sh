#!/bin/bash

if [[ $1 = "" ]]
then
    echo "Errore: inserisci un secondo campo"
elif [[ $1 = "view" ]]
then
    header=$(cat address-book-database.csv | head -n 1)
    body=$(cat address-book-database.csv | tail -n +2 | sort -t ',' -k 4)
    echo -e "$header\n$body" | column -t -s ','
elif [[ $1 = "search" ]]
then
    if [[ $2 = "" ]]
    then
        echo "Errore: inserisci un terzo campo"
    else
        var=$(cat address-book-database.csv | grep $2)
        if [[ $var = "" ]]
        then
            echo "Not Found"
        else
            header=$(cat address-book-database.csv | head -n 1)
            IFS=$','
            read -a array_header <<< "$header"
            IFS=$'\n'
            for line in $var
            do
                IFS=$','
                read -a array <<< "$line"
                len_header=${#array_header[@]}
                for ((i=0; i<$len_header; i++))
                do
                    echo "${array_header[$i]^} : ${array[$i]}"
                done
                echo ""
            done
        fi
    fi
elif [[ $1 = "insert" ]]
then
    echo -n "Name: "
    read name
    echo -n "Surname: "
    read surname
    echo -n "Phone: "
    read phone
    echo -n "Mail: "
    read mail
    if (( $(cat address-book-database.csv | cut -d ',' -f 4 | grep -x $mail | wc -l) > 0))
    then
        echo "Errore: mail già presente"
    else
        echo -n "City: "
        read city
        echo -n "Address: "
        read address
        echo -e -n "\n$name,$surname,$phone,$mail,$city,$address" >> address-book-database.csv
        echo "Added"
    fi
elif [[ $1 = "delete" ]]
then
    if [[ $2 = "" ]]
    then
        echo "Devi inserire un terzo campo"
    else
        if (( $(cat address-book-database.csv | tail -n +2 | cut -d ',' -f 4 | grep -x $2 | wc -l) > 0 ))
        then
            header=$(cat address-book-database.csv | head -n 1)
            body=$(cat address-book-database.csv | tail -n +2 )
            echo "$header" > address-book-database.csv
            IFS=$'\n'
            for line in $body
            do
                if (( $( echo $line | cut -d ',' -f 4 | grep -x $2 | wc -l) == 0 ))
                then
                    echo "$line" >> address-book-database.csv
                fi
            done
            echo "Deleted"
        else
            echo "Cannot find any record"
        fi
    fi
fi


























