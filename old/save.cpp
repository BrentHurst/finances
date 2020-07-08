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
#include "number.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
using namespace std;

void Finances::SaveAccounts(FILE* f,int newyear)
{
	vector<string> v;
	vector<vector<string> > subaccounts;
	AccountMap::iterator mit;
	AccountMap::iterator mit2;

	v.push_back("ACCOUNTS");
	PutLine();
	v.clear();

	for(mit = allaccounts.begin(); mit != allaccounts.end(); mit++)
	{
		v.push_back(mit->second->name); //[i][0]

		//[i][1]
		if(!newyear || mit->second->type==location || mit->second->type==earmark)
			v.push_back(dtos_(mit->second->amount));
		else
			v.push_back(dtos_(0));

		v.push_back(mit->second->t); //[i][2]
		v.push_back(itos_(mit->second->foreign)); //[i][3]
		if(mit->second->foreign)
		{
			v.push_back(dtos_(mit->second->foreignamount)); //[i][4]
			v.push_back(mit->second->foreigncurrency); //[i][5]
		}
		PutLine();
		v.clear();

		if(mit->second->superaccount)
		{
			subaccounts.resize(subaccounts.size()+1);
			subaccounts.back().push_back(mit->second->name);
			subaccounts.back().push_back(mit->second->superaccount->name);
		}
	}
	v.push_back("SUBACCOUNTS");
	PutLine();
	v.clear();

	PutLines();
}

void SaveTransaction(FILE* f,Transaction* t)
{
	vector<string> v;

	v.push_back(itos_(t->date->getTotalDay()));
	v.push_back(t->tag->name);
	v.push_back(t->location->name);
	v.push_back(t->earmark->name);
	v.push_back(t->tofrom->name);
	v.push_back(t->info);
	v.push_back(itos_(t->reconciled));
	v.push_back(dtos_(t->amount));
	v.push_back(itos_(t->foreign));
	if(t->foreign)
	{
		v.push_back(dtos_(t->foreignamount));
		v.push_back(t->foreigncurrency);
	}
	PutLine();
	v.clear();
}

void Finances::SaveTransactions(FILE* f,int newyear)
{
	vector<string> v;
	unsigned int i;

	v.push_back("TRANSACTIONS");
	PutLine();
	v.clear();

	if(newyear)
		return;

	for(i=0; i<transactions.size(); i++)
		if(transactions[i])
			SaveTransaction(f,transactions[i]);
}

void SaveTransfer(FILE* f,Transfer* t)
{
	vector<string> v;

	v.push_back(itos_(t->date->getTotalDay()));
	v.push_back(t->from->name);
	v.push_back(t->to->name);
	v.push_back(t->info);
	v.push_back(itos_(t->reconciled));
	v.push_back(dtos_(t->amount));
	PutLine();
	v.clear();
}

void Finances::SaveTransfers(FILE* f,int newyear)
{
	vector<string> v;
	unsigned int i;

	v.push_back("TRANSFERS");
	PutLine();
	v.clear();

	if(newyear)
		return;

	for(i=0; i<transfers.size(); i++)
		if(transfers[i])
			SaveTransfer(f,transfers[i]);
}

void Finances::SaveRoundUps(FILE* f,int newyear)
{
	vector<string> v;
	AccountMap::iterator mit;
	multimap<double,Account*>::iterator rit;

	v.push_back("ROUND-UPS");
	PutLine();
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
		PutLine();
		v.clear();
	}
}

void Finances::SaveMacros(FILE* f,int newyear)
{
	vector<string> v;
	set<string>::iterator sit;
	unsigned int i;

	v.push_back("MACROS");
	PutLine();
	v.clear();

	for(sit = macronames.begin(); sit != macronames.end(); sit++)
	{
		v.push_back("MACRO");
		v.push_back(*sit);
		PutLine();
		v.clear();

		v.push_back("MTRANSACTIONS");
		PutLine();
		v.clear();
		for(i=0; i<macrotransactions[*sit].size(); i++)
			if(macrotransactions[*sit][i])
				SaveTransaction(f,macrotransactions[*sit][i]);

		v.push_back("MTRANSFERS");
		PutLine();
		v.clear();
		for(i=0; i<macrotransfers[*sit].size(); i++)
			if(macrotransfers[*sit][i])
				SaveTransfer(f,macrotransfers[*sit][i]);
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

	v.push_back(currency);
	PutLine();
	v.clear();

	v.push_back(dtos_(amount));
	PutLine();
	v.clear();

	SaveAccounts(f,newyear);
	SaveTransactions(f,newyear);
	SaveTransfers(f,newyear);
	SaveRoundUps(f,newyear);
	SaveMacros(f,newyear);

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
