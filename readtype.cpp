#include <string.h>
#include "readtype.h"
#include "iodefinesandconsts.h"

string ReadString()
{
	unsigned int SIZE = 100;
	char s[SIZE];
	string str;
	//int junk;
	fgets(s,SIZE,stdin);
	if(strlen(s)>0 && s[strlen(s)-1]=='\n')
		s[strlen(s)-1]='\0';
	//FlushInputBuffer();
	str = s;
	return str;
}

char ReadChar()
{
	char c;
	int junk;
	scanf("%c",&c);
	FlushInputBuffer();
	return c;
}

double ReadDouble()
{
	double d;
	int junk;
	scanf("%lf",&d);
	FlushInputBuffer();
	return d;
}

int ReadInt()
{
	int i;
	int junk;
	scanf("%d",&i);
	FlushInputBuffer();
	return i;
}

string ReadInInformation()
{
	printf("info: ");
	string str = ReadString();
	return str;
}

double ReadInTotal()
{
	double d;
	printf("Amount: (positive for income or transfer, negative for spent): ");
	d = ReadDouble();
	return d;
}
