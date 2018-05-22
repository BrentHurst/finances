/*************************************************
 *   G BRENT HURST
 *   reconcile.cpp
 *   May 13, 2018 (original)
 *   May 13, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include <set>
#include <cstdio>
#include "finances.h"
using namespace std;

int GetDecision()
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

void Finances::Reconcile()
{
	int i = GetDecision();
	Reconcile_(unreconciledtransactions,i);
	Reconcile_(unreconciledtransfers,i);
}

void Account::Reconcile()
{
	Reconcile(GetDecision());
}

void Account::Reconcile(int i)
{
	Reconcile_(unreconciledtransactions,i);
	Reconcile_(unreconciledtransfers,i);
}

void Reconcile_(TransactionSet& s,int i)
{
	TransactionSet::iterator sit;
	TransactionSet::iterator sit2;
	char c;

	for(sit=s.begin(); sit != s.end();   )
	{
		if(!i)
		{
			do
			{
				(*sit)->Print();
				printf("Reconcile this transaction? [y/n]: ");
				c = ReadChar();
			}while(c!='y' && c!='Y' && c!='n' && c!='N');
		}
		printf("\n");

		if(i || c=='y' || c=='Y')
		{
			(*sit)->Reconcile();
			sit2 = sit;
			sit++;
			s.erase(sit2);
		}
		else
			sit++;
	}

	printf("\nSave and quit for your changes to these transactions to take full effect.\n\n");
}

void Reconcile_(TransferSet& s,int i)
{
	TransferSet::iterator sit;
	TransferSet::iterator sit2;
	char c;

	for(sit=s.begin(); sit != s.end();   )
	{
		if(!i)
		{
			do
			{
				(*sit)->Print();
				printf("Reconcile this transfer? [y/n]: ");
				c = ReadChar();
			}while(c!='y' && c!='Y' && c!='n' && c!='N');
		}
		printf("\n");

		if(i || c=='y' || c=='Y')
		{
			(*sit)->Reconcile();
			sit2 = sit;
			sit++;
			s.erase(sit2);
		}
		else
			sit++;
	}

	printf("\nSave and quit for your changes to these transfers to take full effect.\n\n");
}