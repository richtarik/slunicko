#!/bin/bash

help_text="Spatne spusteno."

if [ $# -gt 0 ]
then
	if [ -f $1 ]
	then
		interpret=$1
		pocet_testu = 0
		pocet_uspesnych = 0
		pocet_neuspesnych = 0
		
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
						pocet_testu=$((pocet_testu+1))
						filename=${file##*/}
						echo ""
						echo "		Slozka: "$dirname" Test: "$filename
						echo ""
						
						$interpret "$file"
						ret=$?
						
						if [ $ret != $dirname ]
						then
							pocet_neuspesnych=$((pocet_neuspesnych+1))
							echo ""
							echo "		Spatna navratova hodnota: "$ret
							echo "		Spravna navratova hodnota: "$dirname
						else
							pocet_uspesnych=$((pocet_uspesnych+1))
						fi
						echo ""
						echo "==========================================================="
					fi
				done
			fi
		done
		echo "		SUMMARY:"
		echo ""
		echo "		Testu celkem: "$pocet_testu
		echo "		Pocet uspesnych: "$pocet_uspesnych
		echo "		Pocet neuspesnych: "$pocet_neuspesnych
		echo ""
		echo "		Konec"
	else
		echo $help_text
	fi
else
	echo $help_text
fi
