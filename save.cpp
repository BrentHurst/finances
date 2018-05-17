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

void Finances::SaveAccounts(FILE* f,int newyear)
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

		if(!newyear || mit->second->type==location || mit->second->type==earmark)
			v.push_back(dtos_(mit->second->amount));
		else
			v.push_back(dtos_(0));

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
void Finances::SaveTransactions(FILE* f,int newyear)
{
	vector<string> v;
	TransactionSet::iterator sit;

	v.push_back("TRANSACTIONS");
	PutLine;
	v.clear();

	if(newyear)
		return;

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

void Finances::SaveTransfers(FILE* f,int newyear)
{
	vector<string> v;
	TransferSet::iterator sit;

	v.push_back("TRANSFERS");
	PutLine;
	v.clear();

	if(newyear)
		return;

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

void Finances::SaveRoundUps(FILE* f,int newyear)
{
	vector<string> v;
	map<string,Account*>::iterator mit;
	multimap<double,Account*>::iterator rit;

	v.push_back("ROUND-UPS");
	PutLine;
	v.clear();

	for(mit = allaccounts.begin(); mit != allaccounts.end(); mit++)
	{
		if(mit->second->roundups.empty())
			continue;

		v.push_back(mit->first);
		for(rit = mit->second->roundups.begin(); rit != mit->second->roundups.end(); rit++)
		{
			v.push_back(dtos_(rit->first));
			v.push_back(rit->second->name);
		}
		PutLine;
		v.clear();
	}
}

void Finances::Save()
{
	Save(filename,0);
}

void Finances::Save(const string& fn,int newyear)
{
	FILE* f;
	vector<string> v;

	f = fopen(fn.c_str(),"w");

	v.push_back(dtos_(amount));
	PutLine;

	SaveAccounts(f,newyear);
	SaveTransactions(f,newyear);
	SaveTransfers(f,newyear);
	SaveRoundUps(f,newyear);

	fclose(f);
}

void Finances::NewYear()
{
	string y;
	string fn;

	y = itos_(1 + stoi_(year));

	fn = "finances" + user + y + ".txt";
	Save(fn,1);

	printf("The file for the year %s for the user %s has been created.\n",y.c_str(),user.c_str());
}
