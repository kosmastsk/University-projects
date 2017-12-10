#!/bin/sh
#script for leading the tester of the modify script through the typical, uncommon and even incorrect scenarios of usage of the modify script.

echo "Hello! :) "

while : ; do
	echo "Choose the type of modification - uppercase or lowerize the filenames? [u/l]"
	read mod
	if [ $mod = "u" -o $mod = "U" ]; then
		mod="-u"
		break;
	elif [ $mod = "l" -o $mod = "L" ]; then
		mod="-l"
		break;
	else
		echo " `basename $0` : Error: please enter uppercase or lowerize [u/l]">&2
	fi
done

#set the number of input that we expect
echo "How many files do you want to modify?"
read number

#append each filename to a variable, to save all of them 
echo "Enter each path/filename you want to modify and then press Enter"
files=""
for i in $(seq $number); do
	read filename
	if [ "$(echo $filename | head -c 1)" ]; then
		filename=./$filename
	fi
	files="$files $filename"
done

while : ; do
	echo "Do you want recursive execution or not? [y/n]"
	read rec
	if [ $rec = "y" -o $rec = "Y" ]; then
		rec="-r"
		./modify.sh "$rec" "$mod" "$files"
		break;
	elif [ $rec = "n" -o $rec = "N" ]; then
		./modify.sh "$mod" "$files"
		break;
	else
		echo " `basename $0` : Error: please enter yes or no [y/n]">&2
	fi
done

