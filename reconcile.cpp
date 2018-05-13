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
	int junk;
	do
	{
		printf("Would you like to go ahead and reconcile all, or reconcile one-by-one?\n");
		printf("a. Reconcile all\nb. Reconcile one-by-one\n[a/b]: ");
		scanf("%c",&c);
		FlushInputBuffer;
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

void Reconcile_(multiset<Transaction*>& s,int i)
{
	multiset<Transaction*>::iterator sit;
	multiset<Transaction*>::iterator sit2;
	char c;
	int junk;

	for(sit=s.begin(); sit != s.end();   )
	{
		if(!i)
		{
			do
			{
				(*sit)->Print();
				printf("Reconcile this transaction? [y/n]: ");
				scanf("%c",&c);
				FlushInputBuffer;
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

void Reconcile_(multiset<Transfer*>& s,int i)
{
	multiset<Transfer*>::iterator sit;
	multiset<Transfer*>::iterator sit2;
	char c;
	int junk;

	for(sit=s.begin(); sit != s.end();   )
	{
		if(!i)
		{
			do
			{
				(*sit)->Print();
				printf("Reconcile this transfer? [y/n]: ");
				scanf("%c",&c);
				FlushInputBuffer;
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
