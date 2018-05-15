/*************************************************
 *   G BRENT HURST
 *   global.cpp
 *   December 27, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include <string.h>
#include <cstdio>
#include <cmath>
using namespace std;


void PrintTransactionsGlobal(const multiset<Transaction*>& transactions)
{
	multiset<Transaction*>::iterator iter;

	//need headings

	for(iter=transactions.begin(); iter != transactions.end(); iter++)
		(*iter)->Print();

	printf("\n");
}
void PrintTransfersGlobal(const multiset<Transfer*>& transfers)
{
	multiset<Transfer*>::iterator iter;

	//need headings

	for(iter=transfers.begin(); iter != transfers.end(); iter++)
		(*iter)->Print();

	printf("\n");
}

string dtos_(double d)
{
	char c[100];
	string s;
	sprintf(c,"%f",d);
	s = c;
	return s;
}
string itos_(int i)
{
	char c[100];
	string s;
	sprintf(c,"%d",i);
	s = c;
	return s;
}
double stod_(string s)
{
	double d;
	sscanf(s.c_str(),"%lf",&d);
	return d;
}
int stoi_(string s)
{
	int i;
	sscanf(s.c_str(),"%d",&i);
	return i;
}

double abs(double f) { return (f>=0) ? f : -1*f; }
double Round2Decimals(double d) { return round(d*100)/100; }

string ReadString()
{
	unsigned int SIZE = 100;
	char s[SIZE];
	string str;
	//int junk;
	fgets(s,SIZE,stdin);
	if(strlen(s)>0 && s[strlen(s)-1]=='\n')
		s[strlen(s)-1]='\0';
	//FlushInputBuffer;
	str = s;
	return str;
}

char ReadChar()
{
	char c;
	int junk;
	scanf("%c",&c);
	FlushInputBuffer;
	return c;
}

double ReadDouble()
{
	double d;
	int junk;
	scanf("%lf",&d);
	FlushInputBuffer;
	return d;
}

void PrintCommands(map<char,string>& cmdList)
{
	map<char,string>::iterator mit;

	printf("\n");
	for(mit=cmdList.begin(); mit != cmdList.end(); mit++)
		if(mit->second != "")
			printf("%c. %s\n",mit->first,mit->second.c_str());
}

char GetCommand(map<char,string>& cmdList)
{
	char c;

	PrintCommands(cmdList);

	do
	{
		printf("Please choose a command: ");
		c = ReadChar();
		if(c>='A' && c<='Z')
			c += ('a' - 'A');
	}while(c<'a' || c>'z');

	printf("\n");

	return c;
}
