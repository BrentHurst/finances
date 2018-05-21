/*************************************************
 *   G BRENT HURST
 *   finances.cpp
 *   December 27, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include "iogbh.h"
#include <vector>
#include <cstdio>
using namespace std;

Finances::Finances()
{
	locations.clear();
	earmarks.clear();
	tags.clear();
	tofroms.clear();
	transactions.clear();
	unreconciledtransactions.clear();
	transfers.clear();
	unreconciledtransfers.clear();
	conversions.clear();
	macronames.clear();
	macrotransactions.clear();
	macrotransfers.clear();
	amount=0;
}

void Finances::FindSuperAccount(const string& str,Account* a,map<string,Account*>& m,string type,int setup)
{
	char c='r';

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Does %s have a superaccount? [y/n]: ",str.c_str());
		c = ReadChar();
	}
	if(c=='n' || c=='N') return;

	PrintCorrectAccountMap(type);
	printf("Please enter superaccount name. ");
	a->superaccount = ReadInAccount(m,type,1,setup);
	if(a->superaccount)
		a->superaccount->subaccounts.insert(make_pair(a->name,a));
}

Account* Finances::ReadInAccount(map<string,Account*>& m,string type,int z,int setup)
{
	string str;
	Account* a;
	map<string,Account*>::iterator mit;
	double d;

	while(1)
	{
		PrintCorrectAccountMap(type);
		printf("%s: ",type.c_str());
		str = ReadString();

		//str not in m
		if((mit=m.find(str))==m.end())
		{
			//str in another account map
			if(allaccounts.find(str) != allaccounts.end())
			{
				printf("\"%s\" already exists as a %s.\n",str.c_str(),(*allaccounts.find(str)).second->t.c_str());
				if(!AskForContinue()) return 0;
				else continue;
			}
			//str doesn't exist yet as an account
			else
			{
				PrintCorrectAccountMap(type);
				printf("\n");
				if(AskToAdd(type,str))
				{
					a = new Account(str,type,currency);
					if(setup)
					{
						printf("Enter the starting amount in this %s: %s",type.c_str(),currency.c_str());
						d = ReadDouble();
						a->amount = Round2Decimals(d);
					}
					m[str] = a;
					allaccounts[str] = a;

					FindSuperAccount(str,a,m,type,setup);

					return a;
				}
				else
				{
					if(!AskForContinue()) return 0;
					else continue;
				}
			}
		}

		//str in m
		else
		{
			if(!z)
				printf("\"%s\" already exists.\n",str.c_str());
			return mit->second;
		}
	}
}

string ReadInInformation()
{
	printf("info: ");
	string str = ReadString();
	return str;
}

double ReadInTotal()
{
	double d;
	printf("Amount: (positive for income or transfer, negative for spent): ");
	d = ReadDouble();
	return d;
}

Transaction* Finances::ReadNewTransaction(int link,int fgn)
{
	Date* date;
	Account* tg;
	Account* l;
	Account* e;
	Account* tf;
	string info;
	double t;
	Transaction* transaction;
	string curr;
	char c;

	date = new Date;
	date->ReadInDate();

	if(!(tg = ReadInAccount(tags,"tag",1,0))) {delete date; return NULL;}
	if(!(l = ReadInAccount(locations,"location",1,0))) {delete date; return NULL;}
	if(!(e = ReadInAccount(earmarks,"earmark",1,0))) {delete date; return NULL;}
	if(!(tf = ReadInAccount(tofroms,"to/from",1,0))) {delete date; return NULL;}

	if(l->foreign != fgn || e->foreign != fgn)
	{
		printf("You chose for this to use %s currency.\n",(fgn) ? "foreign" : "your default");
		delete date;
		return NULL;
	}

	info = ReadInInformation();
	t = ReadInTotal();

	transaction = new Transaction(date,tg,l,e,tf,info,0,t,currency);

	if(fgn)
	{
		do
		{
			printf("What currency are you using? (Enter a symbol): ");
			curr = ReadString();
			do
			{
				printf("Is %s correct? [y/n]: ",curr.c_str());
				c = ReadChar();
			}while(c!='y' && c!='Y' && c!='n' && c!='N');
		}while(c=='n' || c=='N');

		transaction->foreign = 1;
		transaction->foreignamount = t;
		transaction->foreigncurrency = curr;

		transaction->amount =
				Round2Decimals
				(
					transaction->foreignamount * conversions[curr].first / conversions[curr].second
				);
	}

	if(!AskIfCorrectTransaction(transaction))
	{
		delete date;
		delete transaction;
		printf("That transaction has been discarded.\n");
		return NULL;
	}
	else if(link)
	{
		LinkTransaction(transaction,0);
		if(fgn)
		{
			conversions[curr].first = Round2Decimals(conversions[curr].first + transaction->amount);
			conversions[curr].second = Round2Decimals(conversions[curr].second + transaction->foreignamount);
		}
		printf("Your transaction has been saved.\n");
		return transaction;
	}
	else
		return transaction;
}

Transfer* Finances::ReadNewTransfer(int link,int fgn)
{
	Date* date;
	Account* from;
	Account* to;
	string info;
	double amount;
	Transfer* t;
	char type='r';

	date = new Date;
	date->ReadInDate();

	while(type != '1' && type != '2')
	{
		printf("If earmarks, enter '1', and if physical locations, enter '2': ");
		type = ReadChar();
	}

	if(type=='1')
	{
		if(!(from = ReadInAccount(earmarks,"from",1,0))) {delete date; return NULL;}
		if(!(to = ReadInAccount(earmarks,"to",1,0))) {delete date; return NULL;}
	}
	else if(type=='2')
	{
		if(!(from = ReadInAccount(locations,"from",1,0))) {delete date; return NULL;}
		if(!(to = ReadInAccount(locations,"to",1,0))) {delete date; return NULL;}
	}

	if(from->foreign != fgn && to->foreign != fgn)
	{
		printf("Both accounts are %s currency.\n",(fgn) ? "your local" : "foreign");
		delete date;
		return NULL;
	}

	info = ReadInInformation();
	amount = Round2Decimals(ReadInTotal());

	t = new Transfer(date,from,to,info,0,amount,currency);

	if(!AskIfCorrectTransfer(t))
	{
		delete date;
		delete t;
		printf("That transfer has been discarded.\n");
		return NULL;
	}
	else if(link)
	{
		LinkTransfer(t,0);
		printf("Your transfer has been saved.\n");
		return t;
	}
	else
		return t;
}

void Finances::ReadNewAccount()
{
	char c='r';

	printf("1. earmark\n2. location\n3. tag\n4. to/from\n");
	while(c<'1' || c>'4')
	{
		printf("Please enter the number of the type of account you are creating: ");
		c = ReadChar();
	}
	switch(c)
	{
		case '1': ReadInAccount(earmarks,"earmark",0,0); break;
		case '2': ReadInAccount(locations,"location",0,0); break;
		case '3': ReadInAccount(tags,"tag",0,0); break;
		case '4': ReadInAccount(tofroms,"to/from",0,0); break;
	}
}

void Finances::RenameAccount(Account* a)
{
	string s;
	char c;

	while(1)
	{
		printf("Please enter what you would like to rename %s to: ",a->name.c_str());
		s = ReadString();
		if(allaccounts.find(s)==allaccounts.end())
			break;
		do
		{
			printf("\"%s\" already exists. Try again? [y/n]: ",s.c_str());
			c = ReadChar();
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='n' || c=='N')
			return;
	}

	allaccounts[s] = a;
	allaccounts.erase(allaccounts.find(a->name));

	if(a->superaccount)
	{
		a->superaccount->subaccounts[s] = a;
		a->superaccount->subaccounts.erase(a->superaccount->subaccounts.find(a->name));
	}

	switch(a->type)
	{
		case location:
					   locations[s] = a;
					   locations.erase(locations.find(a->name));
					   break;
		case earmark:
					   earmarks[s] = a;
					   earmarks.erase(earmarks.find(a->name));
					   break;
		case tag:
					   tags[s] = a;
					   tags.erase(tags.find(a->name));
					   break;
		case tofrom:
					   tofroms[s] = a;
					   tofroms.erase(tofroms.find(a->name));
					   break;
	}

	a->name = s;
}
