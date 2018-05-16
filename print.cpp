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

void Finances::PrintAccountRecur(Account* a,string indent)
{
	map<string,Account*>::iterator mit;

	a->Print(indent);
	for(mit=a->subaccounts.begin(); mit != a->subaccounts.end(); mit++)
		PrintAccountRecur(mit->second,indent+"    ");
}
void Finances::PrintEarmarks()
{
	map<string,Account*>::iterator mit;

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
	map<string,Account*>::iterator mit;

	printf("\n\nLOCATIONS\n---------\n\n");
	printf("%9.2f TOTAL\n",amount);

	for(mit=locations.begin(); mit != locations.end(); mit++)
		if(!mit->second->superaccount)
			PrintAccountRecur(mit->second,"    ");
}
void Finances::PrintTags()
{
	map<string,Account*>::iterator mit;

	printf("\n\nTAGS\n----\n\n");

	for(mit=tags.begin(); mit != tags.end(); mit++)
		if(!mit->second->superaccount)
			PrintAccountRecur(mit->second,"    ");
}
void Finances::PrintTofroms()
{
	map<string,Account*>::iterator mit;

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
