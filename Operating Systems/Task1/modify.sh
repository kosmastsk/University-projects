#!/bin/sh
#script for modifying file names, lowerizing, uppercasing or calling sed command with given sed pattern

#if there are no arguments, exit immediately
[ -z "$1" ] && exit 1;

#help command
if [ $1 = "-h" ]; then
	echo 'Usage:          modify [-r] [-l|-u] <dir/file names...>
	        modify [-h]
Options:
	        -r: recursive execution
	        -l: lowercase filename
	        -u: uppercase filename
	        -h: help options'
exit 0
fi
#exit successfully

#we have recursive execution
if [ $1 = "-r" ]; then
	echo "Recursive execution"
	if [ $2 = "-l" ]; then
		#${3%.} gives only the filename, extracted from the path
		for i in $( echo $3 | grep [A-Z] );
			do mv -i $i `echo $i | tr 'A-Z' 'a-z'`;
		done
		if [ "$#" -gt 3 ]; then
			shift
			shift
			shift
			./modify "-r" "-l" "$@"
		fi
	elif [ $2 = "-u" ]; then
		for i in $( echo $3 | grep [a-z] );
			do mv -i $i `echo $i | tr 'a-z' 'A-Z'`;
		done
		if [ "$#" -gt 3 ]; then
			shift
			shift
			shift
			./modify "-r" "-u" "$@"
		fi
	else
	#error message to standard error output
		echo " `basename $0`: Error: the option $2 you inserted is not valid">&2
	fi

#we do not have recursive execution
else
	echo "Non-recursive execution"
	#check if we want to lowerize or uppecase the letters or call sed command
	#lowerizing
	if [ $1 = "-l" ]; then
		shift
		while [ "$#" -ne 0 ]; do
			for i in $( echo ${1%.} | grep [A-Z] );
				do mv -i $i `echo $i | tr 'A-Z' 'a-z'`;
			done
			shift
		done
	#uppercasing
	elif [ $1 = "-u" ]; then
		shift
		while [ "$#" -ne 0 ]; do
			for i in $( echo ${1%.} | grep [a-z] );
				do mv -i $i `echo $i | tr 'a-z' 'A-Z'`;
			done
			shift
		done
	else
	#error message to the standard error output
		echo " ` basename $0`: Error, the option $1 you inserted is not valid">&2
	fi

fi
