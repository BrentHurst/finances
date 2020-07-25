#include <cstdio>
#include <string>
using namespace std;



int AskToAdd(const string& type,const string& str)
{
	char c='r';

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("The %s \"%s\" doesn't exist. Would you like to add it? [y/n]: ",type.c_str(),str.c_str());
		c = ReadChar();
	}
	return (c=='y' || c=='Y');
}

int AskIfCorrectTransaction(Transaction* t)
{
	char c='r';

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Is the following transaction correct?\n");
		t->Print();
		printf("[y/n]: ");
		c = ReadChar();
	}
	return (c=='y' || c=='Y');
}

int AskIfCorrectTransfer(Transfer* t)
{
	char c='r';

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Is the following transfer correct?\n");
		t->Print();
		printf("[y/n]: ");
		c = ReadChar();
	}
	return (c=='y' || c=='Y');
}

int AskReconcileDecision()
{
	char c;
	do
	{
		printf("Would you like to go ahead and reconcile all, or reconcile one-by-one?\n");
		printf("a. Reconcile all\nb. Reconcile one-by-one\n[a/b]: ");
		c = ReadChar();
	}while(c!='a' && c!='b' && c!='A' && c!='B');
	return (c=='a' || c=='A');
}
