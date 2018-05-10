/*************************************************
 *   G BRENT HURST
 *   global.cpp
 *   December 27, 2017 (original)
 *   December 27, 2017 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include <cstdio>
using namespace std;


void PrintTransactionsGlobal(const multiset<Transaction*>& transactions)
{
	multiset<Transaction*>::iterator iter;

	//need headings

	for(iter=transactions.begin(); iter != transactions.end(); iter++)
		(*iter)->Print();

	printf("\n");
}
void PrintTransfersGlobal(const multiset<Transfer*>& transfers)
{
	multiset<Transfer*>::iterator iter;

	//need headings

	for(iter=transfers.begin(); iter != transfers.end(); iter++)
		(*iter)->Print();

	printf("\n");
}
