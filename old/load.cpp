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
#include "number.h"
#include "readtype.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
using namespace std;

void Finances::LoadAccounts(const vector<vector<string> >& file,int a,int b)
{
	Account* acc;
	int i;
	pair<double,double> zeros;

	zeros.first = 0;
	zeros.second = 0;

	for(i=a; i<b; i++)
	{
		acc = new Account(file[i][0],file[i][2],currency);
		acc->amount = Round2Decimals(stod_(file[i][1]));
		allaccounts.insert(make_pair(acc->name,acc));
		switch(acc->type)
		{
			case location: locations.insert(make_pair(acc->name,acc)); break;
			case earmark:  earmarks.insert(make_pair(acc->name,acc));  break;
			case tag:      tags.insert(make_pair(acc->name,acc));      break;
			case tofrom:   tofroms.insert(make_pair(acc->name,acc));   break;
		}
		if(stoi_(file[i][3]))
		{
			acc->foreign = 1;
			acc->foreignamount = Round2Decimals(stod_(file[i][4]));
			acc->foreigncurrency = file[i][5];

			if(acc->type==location)
			{
				if(conversions.find(acc->foreigncurrency) == conversions.end())
					conversions.insert(make_pair(acc->foreigncurrency,zeros));
				conversions[acc->foreigncurrency].first = Round2Decimals(conversions[acc->foreigncurrency].first + acc->amount);
				conversions[acc->foreigncurrency].second = Round2Decimals(conversions[acc->foreigncurrency].second + acc->foreignamount);
			}
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

Transaction* Finances::LoadTransaction(const vector<vector<string> >& file, int i)
{
	Date* d;
	Transaction* t;
	string s;

	d = new Date;
	d->setWithTotalDay(stoi_(file[i][0]));
	s = file[i][5];
	t = new Transaction(
			nexttransactionid,
			d,
			allaccounts[file[i][1]],
			allaccounts[file[i][2]],
			allaccounts[file[i][3]],
			allaccounts[file[i][4]],
			s,
			stoi_(file[i][6]),
			stod_(file[i][7]),
			currency
			);
	nexttransactionid++;
	if(stoi_(file[i][8]))
	{
		t->foreign = 1;
		t->foreignamount = stod_(file[i][9]);
		t->foreigncurrency = file[i][10];
	}
	return t;
}

void Finances::LoadTransactions(const vector<vector<string> >& file,int a,int b)
{
	int i;
	Transaction* t;

	for(i=a; i<b; i++)
	{
		t = LoadTransaction(file,i);
		LinkTransaction(t,1);
	}

	InsertionSort3(transactions);
}

Transfer* Finances::LoadTransfer(const vector<vector<string> >& file,int i)
{
	Date* d;
	Transfer* t;
	string s;

	d = new Date;
	d->setWithTotalDay(stoi_(file[i][0]));
	s = file[i][3];
	t = new Transfer(
			nexttransferid,
			d,
			allaccounts[file[i][1]],
			allaccounts[file[i][2]],
			s,
			stoi_(file[i][4]),
			stod_(file[i][5]),
			currency
			);
	nexttransferid++;

	return t;
}

void Finances::LoadTransfers(const vector<vector<string> >& file,int a,int b)
{
	int i;
	Transfer* t;
	string s;

	for(i=a; i<b; i++)
	{
		t = LoadTransfer(file,i);
		LinkTransfer(t,1);
	}

	InsertionSort3(transfers);
}

void Finances::LoadRoundUps(const vector<vector<string> >& file,int a,int b)
{
	int i;
	unsigned int j;

	for(i=a; i<b; i++)
		for(j=1; j<file[i].size(); j+=2)
			allaccounts[file[i][0]]->roundups.insert(make_pair(stod_(file[i][j]),allaccounts[file[i][j+1]]));
}

void Finances::LoadMacros(const vector<vector<string> >& file,int a,int b)
{
	int i;
	string n;
	Transaction* t1;
	Transfer* t2;

	for(i=a; i<b; )
	{
		if(file[i][0]=="MACRO")
		{
			n = file[i][1];
			macronames.insert(n);
			i++;
		}

		if(file[i][0]=="MTRANSACTIONS")
		{
			i++;
			while(file[i][0] != "MTRANSFERS")
			{
				t1 = LoadTransaction(file,i);
				PutTransactionInTransactionVec(t1,macrotransactions[n]);
				i++;
			}
		}

		if(file[i][0]=="MTRANSFERS")
		{
			i++;
			while(i<b && file[i][0] != "MACRO")
			{
				t2 = LoadTransfer(file,i);
				PutTransferInTransferVec(t2,macrotransfers[n]);
				i++;
			}
		}
	}
}

void Finances::LoadCheck()
{
	AccountMap::iterator mit;
	AccountMap::iterator mit2;
	double amt;
	string other;

	for(mit = allaccounts.begin(); mit != allaccounts.end(); mit++)
	{
		if(mit->second->subaccounts.empty())
			continue;

		amt = 0;
		for(mit2 = mit->second->subaccounts.begin(); mit2 != mit->second->subaccounts.end(); mit2++)
			amt = Round2Decimals(amt + mit2->second->amount);

		if(abs_(amt) > abs_(mit->second->amount))
		{
			printf("Probably a problem:\n");
			printf("\t%s's subaccounts have a higher abs_(amount) than it does.\n",mit->first.c_str());
		}

		else if(abs_(amt) < abs_(mit->second->amount))
		{
			printf("Probably a problem:\n");
			printf("\t%s's subaccounts have a lower abs_(amount) than it does.\n",mit->first.c_str());
			printf("\tYou should fix the problem by transfering %.2f to \"%s - Other\"\n",
					(amt / abs_(amt))*(abs_(mit->second->amount) - abs_(amt)),
					mit->first.c_str());
		}
	}
}

string Finances::GetFileName()
{
	string s;

	printf("Enter your name: ");
	user = ReadString();

	printf("Enter the year: ");
	year = ReadString();

	s = "finances" + user + year + ".txt";
	return s;
}

void Finances::Load()
{
	FILE* f;
	vector<vector<string> > file;
	unsigned int i;
	int acc = -1;
	int subacc = -1;
	int transac = -1;
	int transfe = -1;
	int ru = -1;
	int m = -1;
	char c='r';

	filename = GetFileName();

	f = fopen(filename.c_str(),"r");
	if(!f)
	{
		while(c!='y' && c!='n' && c!='Y' && c!='N')
		{
			printf("Couldn't find file. Set up new finances? [y/n]: ");
			c = ReadChar();
		}
		if(c=='n' || c=='N')
			exit(0);
		else
		{
			Setup();
			return;
		}
	}

	GetLines();
	fclose(f);

	currency = file[0][0];
	amount = Round2Decimals(stod_(file[1][0]));

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
		else if(file[i][0]=="ROUND-UPS")
			ru = i;
		else if(file[i][0]=="MACROS")
			m = i;
	}

	LoadAccounts(file,acc+1,subacc);
	LoadSubaccounts(file,subacc+1,transac);
	LoadTransactions(file,transac+1,transfe);
	LoadTransfers(file,transfe+1,ru);
	LoadRoundUps(file,ru+1,m);
	LoadMacros(file,m+1,file.size());
	LoadCheck();
}
