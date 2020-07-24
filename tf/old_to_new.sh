#!/bin/bash

if [ $# -ne 1 ]
then
	echo usage: $0 inputfile
	exit 1
fi

tmpfile="dont_use_this_filename"
comma="thisiscommmmmmmmmaaaaaaaaa"
separater="__________"

rm -f $tmpfile

cp $1 $tmpfile

# Get_Line pattern
Get_Line ()
{
	grep -a -n "$1" $tmpfile | sed 's/:.*//'
}

# Delete_Line line
Delete_Line ()
{
	sed -i "$1d" $tmpfile
}

# Find_Last_Line_Of_Section cur_line
Find_Last_Line_Of_Section ()
{
	list=$(Get_Line "ACCOUNTS\|SUBACCOUNTS\|TRANSACTIONS\|TRANSFERS\|END")
	for num in $list
	do
		if [ $num -gt $1 ]
		then
			echo $(($num-1))
			break
		fi
	done
}



# Default Currency
sed -i "1s/.*/\t\"DefaultCurrency\" : \"&\" ,/" $tmpfile

# Start
sed -i "1s/^/{\n/" $tmpfile

# Accounts
line=$(Get_Line "^ACCOUNTS$")
sed -i "${line}s/.*/\t\"Accounts\" : [/" $tmpfile
line=$(($line+1))
lastline=$(Find_Last_Line_Of_Section $line)
range="${line},${lastline}"
sed -i "${range}s/\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)/\t\t{ \"Name\" : \"\1\" , \"Amount\" : \2 , \"Currency\" : \"$\" , \"Type\" : \"\3\" } ,/" $tmpfile
sed -i "${lastline}s/ ,$/\n\t] ,/" $tmpfile


# ParentAccountInfo
line=$(Get_Line "^SUBACCOUNTS$")
sed -i "${line}s/.*/\t\"ParentAccountInfo\" : [/" $tmpfile
line=$(($line+1))
lastline=$(Find_Last_Line_Of_Section $line)
range="${line},${lastline}"
sed -i "${range}s/\(.*\)${separater}\(.*\)/\t\t{ \"Child\" : \"\1\" , \"Parent\" : \"\2\" } ,/" $tmpfile
sed -i "${lastline}s/ ,$/\n\t] ,/" $tmpfile


# Tras - Transactions
line=$(Get_Line "^TRANSACTIONS$")
sed -i "${line}s/.*/\t\"Tras\" : [/" $tmpfile
line=$(($line+1))
lastline=$(Find_Last_Line_Of_Section $line)
range="${line},${lastline}"
sed -i "${range}s/\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)/\t\t{ \"Type\" : \"Transaction\" , \"Id\" : 0 , \"Date\" : \1 , \"Info\" : \"\6\" , \"Reconciled\" : \7 , \"Amount\" : \8 , \"Currency\" : \"$\" , \"DefaultCurrencyAmount\" : \8 , \"Tag\" : \"\2\" , \"Location\" : \"\3\" , \"Earmark\" : \"\4\" , \"ToFrom\" : \"\5\" } ,/" $tmpfile


# Tras - Transfers
line=$(Get_Line "^TRANSFERS$")
Delete_Line $line
lastline=$(Find_Last_Line_Of_Section $line)
range="${line},${lastline}"
sed -i "${range}s/\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)${separater}\(.*\)/\t\t{ \"Type\" : \"Transfer\" , \"Id\" : 0 , \"Date\" : \1 , \"Info\" : \"\4\" , \"Reconciled\" : \5 , \"Amount\" : \6 , \"Currency\" : \"$\" , \"DefaultCurrencyAmount\" : \6 , \"From\" : \"\2\" , \"To\" : \"\3\" } ,/" $tmpfile
sed -i "${lastline}s/ ,$/\n\t] ,/" $tmpfile



# Macros
line=$(Get_Line "^END$")
sed -i "${line}s/^/\t\"Macros\" : [] ,\n/" $tmpfile


# CurrencyConversions
line=$(Get_Line "^END$")
sed -i "${line}s/^/\t\"CurrencyConversions\" : []\n/" $tmpfile


# End
line=$(Get_Line "^END$")
sed -i "${line}s/.*/}/" $tmpfile


cat $tmpfile
rm -f $tmpfile
