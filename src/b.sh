#!/bin/bash
cd XCC
for file in ./*
do
    if test -f $file
    then 
	echo $file is a file
    fi
    if test -d $file
    then 
        echo $file is a dir
	cd $file
	if
	cp ${file}_rm20bp_edinormalized.tlx /home
	cd -
    fi
done
