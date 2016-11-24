#!/bin/bash

#########################################################################
#
# Skript na test interpretu
# V podslozkach s cisly navratovych hodnot jsou zdrojove kody.
#
# Navod na argumenty skriptu:
#   prvni argument ... nazev spustitelneho interpretu
#   -v ... pouziti valgrindu
#   -i ... pozastaveni, kdyz se valgrindu neco nezda nebo 
#          neocekavana vystupni hodnota, nasledne moznost pokracovat
#   -a ... skript ceka za testy s ruznou navratovou hodnotou na kontrolu
#
#########################################################################

help_text="Spatne zadano, navod ve skriptu."

if [ $# -gt 0 ]
then

	if [ -f $1 ]
	then
		echo "===============Start==============="
		
		error_files=""
		error_files_val=""
		flag=0
		flag1=0
		flag2=0
		val_find=0
		bad_ret=0
		con=0
		
		for arg in $*
		do
			if [ $arg == "-v" ]
			then
				flag=1
			fi
			
			if [ $arg == "-i" ]
			then
				flag1=1
			fi
			
			if [ $arg == "-a" ]
			then
				flag2=1
			fi
		done
		
		if [ $flag -eq 1 ]
		then
			echo ""
			echo "	!!!Kontrola s valgrindem!!!"
			echo ""
			echo "==================================="
		fi
		
		echo " Start chybných zdrojáků"
		
		for dir in 1 2 3 4 6 8 9 10
		do
			if [ -d $dir ]
			then
				echo ""
				echo "		==========="
				echo "		 Testy "$dir
				echo "		==========="
				
				for file in $dir/*\.test
				do
					if [ -f $file ]
					then
						echo ""
						echo "		Test '"$file"'"
					
						./$1 "$file"
						ret=$?
						
						line=0
						con=0
						
						if [ $ret != $dir ]
						then
							echo ""
							echo "			spravny vystup: "$dir
							echo "			vystup programu: "$ret
							echo ""
							error_files=$error_files" "$file
							bad_ret=1
							line=1
							con=1
						fi
						
						if [ $flag -eq 1 ]
						then
							valgrind --log-file="$file".valgrind_out --error-exitcode=42 --leak-check=full ./$1 "$file"
							val=$?
							
							if [ $val -eq 42 ]
							then
								echo ""
								echo "		Valgrind '"$file"':"
								cat "$file".valgrind_out | sed 's/^.*[=]/\t/g' | grep "bytes"
								cat "$file".valgrind_out | sed 's/^.*[=]/\t/g' | grep "ERROR SUMMARY"
								echo ""
								val_find=1
								error_files_val=$error_files_val" "$file
								line=2
								con=1
							fi
							
							rm "$file".valgrind_out
						fi
						
						if [ $line -eq 1 ]
						then
							echo "		==========================="
							echo ""
						fi
						
						if [ $line -eq 2 ]
						then
							echo "	================================================================="
							echo ""
						fi
						
						if [ $flag1 -eq 1 ]
						then
							if [ $con -eq 1 ]
							then
								echo "		Pokracovat?"
								read a
								if [ "$a" == 'n' ]
								then
									exit 1
								fi
							fi
						fi
					fi
				done
			fi
		done
		
		echo ""
		echo " Konec chybných zdrojáků"
		echo "==================================="
		echo " Start testu s ruznymi navratovymi hodnotami"
		
		for file in 0/*\.test
		do
			if [ -f $file ]
			then
				echo ""
				echo "		Test '"$file"'"
				echo ""
				
				./$1 "$file"
				ret=$?
				
				echo ""
				echo "		Navratova hodnota testu '"$file"': "$ret
				echo ""
				
				if [ $flag2 -eq 1 ]
				then
					echo "		Pokracovat?"
					read b
					if [ "$b" == 'n' ]
					then
						exit 1
					fi
				fi
				
				line=0
				con=0
				
				if [ $flag -eq 1 ]
				then
					valgrind --log-file="$file".valgrind_out --error-exitcode=42 --leak-check=full ./$1 "$file"
					val=$?
					
					if [ $val -eq 42 ]
					then
						echo ""
						echo "		Valgrind '"$file"':"
						cat "$file".valgrind_out | sed 's/^.*[=]/\t/g' | grep "bytes"
						cat "$file".valgrind_out | sed 's/^.*[=]/\t/g' | grep "ERROR SUMMARY"
						echo ""
						error_files_val=$error_files_val" "$file
						val_find=1
						line=1
						con=1
					fi
					
					rm "$file".valgrind_out
				fi
				
				if [ $line -eq 1 ]
				then
					echo "	================================================================="
				fi
				
				if [ $line -eq 0 ]
				then
					echo "		======================================"
				fi
				
				if [ $flag1 -eq 1 ]
				then
					if [ $con -eq 1 ]
					then
						echo "		Pokracovat?"
						read a
						if [ "$a" == 'n' ]
						then
							exit 1
						fi
					fi
				fi
			fi
		done
		
		echo ""
		echo " Konec testu s ruznymi navratovymi hodnotami"
		echo "==================================="
		echo " SUMMARY:"
		echo ""
		
		sum=0
		
		if [ $bad_ret -eq 1 ]
		then
			echo "	Soubory se spanou vystupni hodnotou:"
			echo "	"$error_files
			echo ""
			sum=1
		fi
		
		if [ $flag -eq 1 ]
		then
			if [ $val_find -eq 1 ]
			then
				echo "	Soubory, kde valgrind neco nasel:"
				echo "	"$error_files_val
				echo ""
				sum=1
			fi
		fi
		
		if [ $sum -eq 0 ]
		then
			echo "	Probably OK"
			echo ""
		fi
		
		echo "===============Konec==============="
	else
		echo $help_text
	fi
else
	echo $help_text
fi
