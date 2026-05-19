#!/bin/bash
if [[ $1 = "" || $2 = "" ]] 
then
    echo "numero argomenti insufficienti"
else
    WORKDIR="/tmp/tmp-so"
    mkdir -p $WORKDIR

    file=$(cat $1)

    IFS=$'\n'

    for line in $file
    do
        ORIGIN=$(echo $line | cut -d " " -f 1)
        DEST=$(echo $line | cut -d " " -f 2)
        if [[ -d $ORIGIN || -d $DEST ]]
        then
            mkdir -p $WORKDIR$DEST
            bindfs --no-allow-other $ORIGIN $WORKDIR$DEST
        elif [[ -f $ORIGIN || -f $DEST ]]
        then
            dir=$(echo $DEST | cut -d "/" -f 2)
            mkdir -p $WORKDIR/$dir
            cp $ORIGIN $WORKDIR/$dir
        fi    
    done

    COMMAND="${@:2}"
    fakechroot chroot $WORKDIR $COMMAND
fi

