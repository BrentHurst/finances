#include "reconcile.h"
#include "readtype.h"
#include <cstdio>
using namespace std;

void Reconcile_(TransactionVec& tv,int a)
{
	unsigned int i;
	char c;


	for(i=0; i<tv.size(); i++)
	{
		if(tv[i])
		{
			if(!a)
			{
				do
				{
					tv[i]->Print();
					printf("Reconcile this transaction? [y/n]: ");
					c = ReadChar();
				}while(c!='y' && c!='Y' && c!='n' && c!='N');
				printf("\n");
			}

			if(a || c=='y' || c=='Y')
			{
				tv[i]->Reconcile();
				RemoveTransactionFromTransactionVec(tv[i],tv);
			}
		}
	}

	printf("\nSave and quit for your changes to these transactions to take full effect.\n\n");
}

void Reconcile_(TransferVec& tv,int a)
{
	unsigned int i;
	char c;


	for(i=0; i<tv.size(); i++)
	{
		if(tv[i])
		{
			if(!a)
			{
				do
				{
					tv[i]->Print();
					printf("Reconcile this transfer? [y/n]: ");
					c = ReadChar();
				}while(c!='y' && c!='Y' && c!='n' && c!='N');
				printf("\n");
			}

			if(a || c=='y' || c=='Y')
			{
				tv[i]->Reconcile();
				RemoveTransferFromTransferVec(tv[i],tv);
			}
		}
	}

	printf("\nSave and quit for your changes to these transfers to take full effect.\n\n");
}
