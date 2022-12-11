#!/usr/bin/bash

basedir="${1%%.*}"

if [[ -d $basedir ]];then
    echo "*** $basedir already exists"
    exit 1
fi

mkdir $basedir
cd $basedir
7z x "$1"


