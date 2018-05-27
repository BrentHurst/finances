#include "printtra.h"
#include <cstdio>
using namespace std;

void PrintTransactionsGlobal(const TransactionVec& transactions)
{
	unsigned int i;

	//need headings

	for(i=0; i<transactions.size(); i++)
		if(transactions[i])
			transactions[i]->Print();

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
