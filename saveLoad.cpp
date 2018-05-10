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
#define PutLines PutDelimitedFile(f,c0,c1,esc,subaccounts)
#define GetLine GetDelimitedLine(f,c0,c1,esc,v)
#define GetLines GetDelimitedFile(f,c0,c1,esc,file)

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

void Finances::LoadAccounts(const vector<vector<string> >& file,int a,int b)
{
	Account* acc;
	int i;

	for(i=a, i<b; i++)
	{
		acc = new Account(file[i][0],file[i][2]);
		acc->amount = stod(file[i][1]);
		allaccounts.insert(make_pair(acc->name,acc));
		switch(acc->type)
		{
			case location: locations.insert(make_pair(acc->name,acc)); break;
			case earmark:  earmarks.insert(mark_pair(acc->name,acc));  break;
			case tag:      tags.insert(make_pair(acc->name,acc));      break;
			case tofrom:   tofroms.insert(mark_pair(acc->name,acc));   break;
		}
	}
}

void Finances::LoadSubaccounts(const vector<vector<string> >& file,int a,int b)
{
	Account* acc;
	int i,j;

	for(i=a; i<b; i++)
	{
		acc = file[i][0];
		acc->superaccount = allaccounts[file[i][1]];
		allaccounts[file[i][1]]->subaccounts.insert(make_pair(acc->name,acc));
	}
}

void Finances::LoadTransactions(const vector<vector<string> >& file,int a,int b)
{
	int i;
	Transaction* t;
	Date* d;

	for(i=a; i<b; i++)
	{
		d = new Date;
		d->setWithTotalDay(stoi(file[i][0]));
		t = new Transaction(
								d,
								allaccounts[file[i][1]],
								allaccounts[file[i][2]],
								allaccounts[file[i][3]],
								allaccounts[file[i][4]],
								file[i][5],
								stoi(file[i][6]),
								stod(file[i][7])
						   );
		LinkTransaction(t);
	}
}

void Finances::LoadTransfers(const vector<vector<string> >& file,int a,int b)
{
	int i;
	Transfer* t;
	Date* d;

	for(i=a; i<b; i++)
	{
		d = new Date;
		d->setWithTotalDay(stoi(file[i][0]));
		t = new Transfer(
							d,
							allaccounts[file[i][1]],
							allaccounts[file[i][2]],
							file[i][3],
							stoi(file[i][4]),
							stod(file[i][5])
						);
		LinkTransfer(t);
	}
}

void Finances::Load(const string& filename)
{
	FILE* f;
	vector<vector<string> > file;
	int i;
	int acc = -1;
	int subacc = -1;
	int transac = -1;
	int transfe = -1;
	char c='r';


	f = fopen(filename.c_str(),"r");
	if(!GetLines)
	{
		while(c!='y' && c!='n' && c!='Y' && c!='N')
		{
			printf("Couldn't find file. Set up new finances? [y/n]: ");
			scanf("%c\n",&c);
		}
		if(c=='n' || c=='N')
			exit(0);
		else
		{
			Setup(filename);
			return;
		}
	}
	fclose(f);

	amount = stod(file[0][0]);

	for(i=0; i<file.size(); i++)
	{
		if(file[i][0]=="ACCOUNTS")
			acc = i;
		else if(file[i][0]=="SUBACCOUNTS")
			subacc = i;
		else if(file[i][0]=="TRANSACTIONS")
			transac = i;
		else if(file[i][0]=="TRANSFERS")
			transfe = i;
	}

	LoadAccounts(file,acc+1,subacc);
	LoadSubaccounts(file,subacc+1,transac);
	LoadTransactions(file,transac+1,transfe);
	LoadTransfers(file,transfe+1,file.size());
}

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
		v.push_back(dtos(mit->second->amount));
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
		v.push_back(itos((*sit)->reconciled));
		v.push_back(dtos((*sit)->amount));
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
		v.push_back(itos((*sit)->reconciled));
		v.push_back(dtos((*sit)->amount));
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
