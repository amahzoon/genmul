#!/bin/bash

if [ "$#" -lt "1" ] 
then 
    echo 
    echo $0: Error: path argument is missing! Abort!
    echo
    exit 1
fi

echo $0: generate compilation time string and SHA256 message digest

VERFILE="$1/"version.hppgen

echo > $VERFILE

#TIME=`env TZ=Europe/Berlin date +"%a, %b %d %Y, %H:%M:%S"`
TIME=`date +"%Y-%m-%d %H:%M:%S"`

#sort files
L=`ls "$1/"*.cpp "$1/"*.hpp | sort`
S1=`sha256sum $L > "$1/"key`


file="$1/key"
fileout="$1/key2"
rm -f $fileout

#this removes the local paths to each src file
while IFS= read -r line
do
	#printf '%s\n' "$line"

        S=`echo $line | awk '{ print $1 }' `
        F=`echo $line | awk '{ print $2 }' `
 
        RP=${F##*/}
        echo $S " "$RP >> $fileout


done <"$file"



S2=`sha256sum $fileout`

#S2=${S2%" key"}

S2=${S2%% *}

#rm "$1/"key

echo \#include\<string\> >> $VERFILE

echo '\n' >> $VERFILE

echo const std::string GLOBAL_COMPILATION_TIME = \"$TIME\"\; >> $VERFILE

echo const std::string GLOBAL_SHA256 = \"$S2\"\; >> $VERFILE

#write detailed sha256 values

echo -n 'const std::string GLOBAL_SHA256_DETAILED = "' >> $VERFILE
while IFS= read -r line
do
	#printf '%s\n' "$line"
        echo -n $line'\\n' >>$VERFILE

done <"$fileout"

echo '";' >>$VERFILE

