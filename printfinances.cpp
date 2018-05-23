/*************************************************
 *   G BRENT HURST
 *   print.cpp
 *   May 10, 2018 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
#include "printtra.h"
#include <cstdio>
using namespace std;

void Finances::PrintTransactions()
{
	PrintTransactionsGlobal(transactions);
}
void Finances::PrintTransfers()
{
	PrintTransfersGlobal(transfers);
}

void Finances::PrintUnreconciledTransactions()
{
	PrintTransactionsGlobal(unreconciledtransactions);
}
void Finances::PrintUnreconciledTransfers()
{
	PrintTransfersGlobal(unreconciledtransfers);
}

void Finances::PrintAccountRecur(Account* a,string indent)
{
	AccountMap::iterator mit;

	a->Print(indent);
	for(mit=a->subaccounts.begin(); mit != a->subaccounts.end(); mit++)
		PrintAccountRecur(mit->second,indent+"    ");
}
void Finances::PrintEarmarks()
{
	AccountMap::iterator mit;

	printf("\n\nEARMARKS\n--------\n\n");
	printf("%9.2f TOTAL\n",amount);

	for(mit=earmarks.begin(); mit != earmarks.end(); mit++)
		if(mit->second && !mit->second->superaccount)
			PrintAccountRecur(mit->second,"    ");
		else if(!mit->second)
		{
			printf("Error here, mit->first is %s\n",mit->first.c_str());
		}
}
void Finances::PrintLocations()
{
	AccountMap::iterator mit;

	printf("\n\nLOCATIONS\n---------\n\n");
	printf("%9.2f TOTAL\n",amount);

	for(mit=locations.begin(); mit != locations.end(); mit++)
		if(!mit->second->superaccount)
			PrintAccountRecur(mit->second,"    ");
}
void Finances::PrintTags()
{
	AccountMap::iterator mit;

	printf("\n\nTAGS\n----\n\n");

	for(mit=tags.begin(); mit != tags.end(); mit++)
		if(!mit->second->superaccount)
			PrintAccountRecur(mit->second,"    ");
}
void Finances::PrintTofroms()
{
	AccountMap::iterator mit;

	printf("\n\nTO/FROMS\n--------\n\n");

	for(mit=tofroms.begin(); mit != tofroms.end(); mit++)
		if(!mit->second->superaccount)
			PrintAccountRecur(mit->second,"    ");
}
void Finances::PrintAllAccounts()
{
	PrintEarmarks();
	PrintLocations();
	PrintTags();
	PrintTofroms();
}

void Finances::PrintCorrectAccountMap(const string& type)
{
	if(type=="location")
		PrintLocations();
	else if(type=="earmark")
		PrintEarmarks();
	else if(type=="tag")
		PrintTags();
	else if(type=="to/from")
		PrintTofroms();
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

void Finances::PrintMacros()
{
	set<string>::iterator sit;
	TransactionSet::iterator tsit1;
	TransferSet::iterator tsit2;

	if(macronames.empty())
	{
		printf("There aren't any macros.\n");
		return;
	}

	for(sit=macronames.begin(); sit != macronames.end(); sit++)
	{
		printf("%s\n",(*sit).c_str());

		for(tsit1 = macrotransactions[*sit].begin(); tsit1 != macrotransactions[*sit].end(); tsit1++)
			(*tsit1)->Print();
		for(tsit2 = macrotransfers[*sit].begin(); tsit2 != macrotransfers[*sit].end(); tsit2++)
			(*tsit2)->Print();

		printf("\n");
	}
}
