/*************************************************
 *   G BRENT HURST
 *   finances.cpp
 *   May 11, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
#include <cstdio>
using namespace std;


int AskForContinue()
{
	char c = 'r';
	int junk;

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Would you like to try again? [y/n]: ");
		scanf("%c",&c);
		FlushInputBuffer;
	}
	return (c=='y' || c=='Y');
}

int AskToAdd(const string& type,const string& str)
{
	char c='r';
	int junk;

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("The %s \"%s\" doesn't exist. Would you like to add it? [y/n]: ",type.c_str(),str.c_str());
		scanf("%c",&c);
		FlushInputBuffer;
	}
	return (c=='y' || c=='Y');
}

int AskIfCorrectTransaction(Transaction* t)
{
	char c='r';
	int junk;

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Is the following transaction correct?\n");
		t->Print();
		printf("[y/n]: ");
		scanf("%c",&c);
		FlushInputBuffer;
	}
	return (c=='y' || c=='Y');
}

int AskIfCorrectTransfer(Transfer* t)
{
	char c='r';
	int junk;

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Is the following transfer correct?\n");
		t->Print();
		printf("[y/n]: ");
		scanf("%c",&c);
		FlushInputBuffer;
	}
	return (c=='y' || c=='Y');
}
