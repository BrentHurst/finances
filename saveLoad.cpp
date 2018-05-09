/*************************************************
 *   G BRENT HURST
 *   saveLoad.cpp
 *   December 27, 2017 (original)
 *   May 8, 2018 (last edit)
 *
 *   #include "finances.h"
 *   #include "iogbh.h"
 *
 ************************************************/


#include "finances.h"
#include "iogbh.h"
#include <vector>
#include <cstdio>
using namespace std;

const char c0 = '\0';
const int c1 = '\n';
const int esc = '`';

#define PutLine PutDelimitedLine(f,c0,c1,esc,v)
#define GetLine GetDelimitedLine(f,c0,c1,esc,v)

string dtos(double d)
{
	char c[100];
	string s;
	sprintf(c,"%f",d);
	s = c;
	return s;
}
string itos(int i)
{
	char c[100];
	string s;
	sprintf(c,"%d",i);
	s = c;
	return s;
}
double stod(string& s)
{
	double d;
	sscanf(s.c_str(),"%lf",&d);
	return d;
}
int stoi(string& s)
{
	int i;
	sscanf(s.c_str(),"%d",&i);
	return i;
}

void LoadGenericAccount(FILE* f,map<string,Account*>& m)
{
	vector<string> v;
	Account* acc;

	while(GetLine && v[0] != "TRANSACTIONS")
	{
		acc = new Account(v[0],v[2]);
		acc->amount = stod(v[1]);
		v.clear();

		if(!GetLine)
			//error
		if(v[0] != "")
			//set superaccount
		//set subaccounts
		v.clear();
	}
}

void Finances::LoadAccounts(FILE* f)
{
}

void Finances::LoadTransactions(FILE* f)
{

}

void Finances::LoadTransfers(FILE* f)
{

}

void Finances::Load(const string& filename)
{
	FILE* f;
	vector<string> v;

	f = fopen(filename.c_str(),"r");

	if(!GetLine)
		//error
	amount = stod(v[0]);

	LoadAccounts(f);
	LoadTransactions(f);
	LoadTransfers(f);

	fclose(f);
}

void SaveGenericAccount(FILE* f,map<string,Account*>& m)
{
	vector<string> v;
	map<string,Account*>::iterator mit;
	map<string,Account*>::iterator mit2;

	for(mit = m.begin(); mit != m.end(); mit++)
	{
		v.push_back(mit->second->name);
		v.push_back(dtos(mit->second->amount));
		v.push_back(mit->second->t);
		PutLine;
		v.clear();

		if(mit->second->superaccount)
			v.push_back(mit->second->superaccount->name);
		else
			v.push_back("");
		for(mit2=mit->second->subaccounts.begin(); mit2 != mit->second->subaccounts.end(); mit++)
			v.push_back(mit2->second->name);
		PutLine;
		v.clear();
	}
}

void Finances::SaveAccounts(FILE* f)
{
	vector<string> v;
	v.push_back("ACCOUNTS");
	PutLine;

	SaveGenericAccount(f,locations);
	SaveGenericAccount(f,earmarks);
	SaveGenericAccount(f,tags);
	SaveGenericAccount(f,tofroms);
}
void Finances::SaveTransactions(FILE* f)
{
	vector<string> v;
	multiset<Transaction*>::iterator sit;

	v.push_back("TRANSACTIONS");
	PutLine;
	v.clear();

	for(sit = transactions.begin(); sit != transactions.end(); sit++)
	{
		v.push_back(itos((*sit)->date->getTotalDay()));
		v.push_back((*sit)->tag->name);
		v.push_back((*sit)->location->name);
		v.push_back((*sit)->earmark->name);
		v.push_back((*sit)->tofrom->name);
		v.push_back((*sit)->info);
		v.push_back(dtos((*sit)->amount));
		v.push_back(itos((*sit)->reconciled));
		PutLine;
		v.clear();
	}
}

void Finances::SaveTransfers(FILE* f)
{
	vector<string> v;
	multiset<Transfer*>::iterator sit;

	v.push_back("TRANSFERS");
	PutLine;
	v.clear();

	for(sit = transfers.begin(); sit != transfers.end(); sit++)
	{
		v.push_back(itos((*sit)->date->getTotalDay()));
		v.push_back((*sit)->from->name);
		v.push_back((*sit)->to->name);
		v.push_back((*sit)->info);
		v.push_back(dtos((*sit)->amount));
		v.push_back(itos((*sit)->reconciled));
		PutLine;
		v.clear();
	}
}

void Finances::Save(const string& filename)
{
	FILE* f;
	vector<string> v;

	f = fopen(filename.c_str(),"w");

	v.push_back(dtos(amount));
	PutLine;

	SaveAccounts(f);
	SaveTransactions(f);
	SaveTransfers(f);

	fclose(f);
}
