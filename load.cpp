/*************************************************
 *   G BRENT HURST
 *   load.cpp
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

void Finances::LoadAccounts(const vector<vector<string> >& file,int a,int b)
{
	Account* acc;
	int i;

	for(i=a; i<b; i++)
	{
		acc = new Account(file[i][0],file[i][2]);
		acc->amount = stod_(file[i][1]);
		allaccounts.insert(make_pair(acc->name,acc));
		switch(acc->type)
		{
			case location: locations.insert(make_pair(acc->name,acc)); break;
			case earmark:  earmarks.insert(make_pair(acc->name,acc));  break;
			case tag:      tags.insert(make_pair(acc->name,acc));      break;
			case tofrom:   tofroms.insert(make_pair(acc->name,acc));   break;
		}
	}
}

void Finances::LoadSubaccounts(const vector<vector<string> >& file,int a,int b)
{
	Account* acc;
	int i;

	for(i=a; i<b; i++)
	{
		acc = allaccounts[file[i][0]];
		acc->superaccount = allaccounts[file[i][1]];
		allaccounts[file[i][1]]->subaccounts.insert(make_pair(acc->name,acc));
	}
}

void Finances::LoadTransactions(const vector<vector<string> >& file,int a,int b)
{
	int i;
	Transaction* t;
	Date* d;
	string s;

	for(i=a; i<b; i++)
	{
		d = new Date;
		d->setWithTotalDay(stoi_(file[i][0]));
		s = file[i][5];
		t = new Transaction(
								d,
								allaccounts[file[i][1]],
								allaccounts[file[i][2]],
								allaccounts[file[i][3]],
								allaccounts[file[i][4]],
								s,
								stoi_(file[i][6]),
								stod_(file[i][7])
						   );
		LinkTransaction(t,1);
	}
}

void Finances::LoadTransfers(const vector<vector<string> >& file,int a,int b)
{
	int i;
	Transfer* t;
	Date* d;
	string s;

	for(i=a; i<b; i++)
	{
		d = new Date;
		d->setWithTotalDay(stoi_(file[i][0]));
		s = file[i][3];
		t = new Transfer(
							d,
							allaccounts[file[i][1]],
							allaccounts[file[i][2]],
							s,
							stoi_(file[i][4]),
							stod_(file[i][5])
						);
		LinkTransfer(t,1);
	}
}

void Finances::Load(const string& filename)
{
	FILE* f;
	vector<vector<string> > file;
	unsigned int i;
	int acc = -1;
	int subacc = -1;
	int transac = -1;
	int transfe = -1;
	char c='r';


	f = fopen(filename.c_str(),"r");
	if(!f)
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

	GetLines;
	amount = stod_(file[0][0]);

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
