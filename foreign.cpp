/*************************************************
 *   G BRENT HURST
 *   foreign.cpp
 *   May 19, 2018 (original)
 *   May 19, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
using namespace std;

void Finances::ReadNewForeignTransaction()
{
	ReadNewTransaction(1,1);
}

void Finances::ReadTransferToForeign()
{
	ReadNewTransfer(1,1);
}

void Finances::ReadTransferFromForeign()
{
	ReadNewTransfer(1,2);
}

void Finances::PrintForeignTransactions()
{
	TransactionSet::iterator sit;
	for(sit=transactions.begin(); sit != transactions.end(); sit++)
		if((*sit)->foreign)
			(*sit)->Print();
}

void Finances::PrintForeignTransfers()
{
	TransferSet::iterator sit;
	for(sit=transfers.begin(); sit != transfers.end(); sit++)
		if((*sit)->foreign)
			(*sit)->Print();
}
