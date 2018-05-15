/*************************************************
 *   G BRENT HURST
 *   save.cpp
 *   December 27, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *   #include "iogbh.h"
 *
 ************************************************/


#include "finances.h"
#include "iogbh.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
using namespace std;

void Finances::SaveAccounts(FILE* f)
{
	vector<string> v;
	vector<vector<string> > subaccounts;
	map<string,Account*>::iterator mit;
	map<string,Account*>::iterator mit2;

	v.push_back("ACCOUNTS");
	PutLine;
	v.clear();

	for(mit = allaccounts.begin(); mit != allaccounts.end(); mit++)
	{
		v.push_back(mit->second->name);
		v.push_back(dtos_(mit->second->amount));
		v.push_back(mit->second->t);
		PutLine;
		v.clear();

		if(mit->second->superaccount)
		{
			subaccounts.resize(subaccounts.size()+1);
			subaccounts.back().push_back(mit->second->name);
			subaccounts.back().push_back(mit->second->superaccount->name);
		}
	}
	v.push_back("SUBACCOUNTS");
	PutLine;
	v.clear();

	PutLines;
}
void Finances::SaveTransactions(FILE* f)
{
	vector<string> v;
	TransactionSet::iterator sit;

	v.push_back("TRANSACTIONS");
	PutLine;
	v.clear();

	for(sit = transactions.begin(); sit != transactions.end(); sit++)
	{
		v.push_back(itos_((*sit)->date->getTotalDay()));
		v.push_back((*sit)->tag->name);
		v.push_back((*sit)->location->name);
		v.push_back((*sit)->earmark->name);
		v.push_back((*sit)->tofrom->name);
		v.push_back((*sit)->info);
		v.push_back(itos_((*sit)->reconciled));
		v.push_back(dtos_((*sit)->amount));
		PutLine;
		v.clear();
	}
}

void Finances::SaveTransfers(FILE* f)
{
	vector<string> v;
	TransferSet::iterator sit;

	v.push_back("TRANSFERS");
	PutLine;
	v.clear();

	for(sit = transfers.begin(); sit != transfers.end(); sit++)
	{
		v.push_back(itos_((*sit)->date->getTotalDay()));
		v.push_back((*sit)->from->name);
		v.push_back((*sit)->to->name);
		v.push_back((*sit)->info);
		v.push_back(itos_((*sit)->reconciled));
		v.push_back(dtos_((*sit)->amount));
		PutLine;
		v.clear();
	}
}

void Finances::Save(const string& filename)
{
	FILE* f;
	vector<string> v;

	f = fopen(filename.c_str(),"w");

	v.push_back(dtos_(amount));
	PutLine;

	SaveAccounts(f);
	SaveTransactions(f);
	SaveTransfers(f);

	fclose(f);
}
