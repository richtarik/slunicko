#!/bin/bash

help_text="Spatne spusteno."

if [ $# -gt 0 ]
then
	if [ -f $1 ]
	then
		interpret=$1
		
		echo "		Spoustim testy"
		echo "==========================================================="
		for dir in testy/*
		do
			dirname=${dir##testy/}
			if [ -d $dir ]
			then
				for file in $dir/*\.test
				do
					if [ -f $file ]
					then
						filename=${file##*/}
						echo ""
						echo "		Slozka: "$dirname" Test: "$filename
						echo ""
						
						$interpret "$file"
						ret=$?
						
						if [ $ret != $dirname ]
						then
							echo ""
							echo "		Spatna navratova hodnota: "$ret
							echo "		Spravna navratova hodnota: "$dirname
						fi
						echo ""
						echo "==========================================================="
					fi
				done
			fi
		done
		echo "		Konec"
	else
		echo $help_text
	fi
else
	echo $help_text
fi
