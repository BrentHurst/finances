#include "printtra.h"
#include <cstdio>
using namespace std;

void PrintTransactionsGlobal(const TransactionSet& transactions)
{
	TransactionSet::iterator iter;

	//need headings

	for(iter=transactions.begin(); iter != transactions.end(); iter++)
		(*iter)->Print();

	printf("\n");
}
void PrintTransfersGlobal(const TransferVec& transfers)
{
	unsigned int i;

	//need headings

	for(i=0; i<transfers.size(); i++)
		if(transfers[i])
			transfers[i]->Print();

	printf("\n");
}
