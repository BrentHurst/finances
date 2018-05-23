#include "reconcile.h"
#include "readtype.h"
using namespace std;

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
