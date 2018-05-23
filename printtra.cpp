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
void PrintTransfersGlobal(const TransferSet& transfers)
{
	TransferSet::iterator iter;

	//need headings

	for(iter=transfers.begin(); iter != transfers.end(); iter++)
		(*iter)->Print();

	printf("\n");
}
