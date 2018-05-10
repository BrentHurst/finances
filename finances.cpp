/*************************************************
 *   G BRENT HURST
 *   finances.cpp
 *   December 27, 2017 (original)
 *   May 8, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include "iogbh.h"
#include <vector>
#include <cstdio>
#include <iostream>
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
	amount=0;
}

int AskForContinue()
{
	char c = 'r';
	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Would you like to try again? [y/n]: ");
		scanf("%c\n",&c);
	}
	return (c=='y' || c=='Y');
}

int AskToAdd(const string& type,const string& str)
{
	char c='r';

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("The %s \"%s\" doesn't exist. Would you like to add it? [y/n]: ",type.c_str(),str.c_str());
		scanf("%c\n",&c);
	}
	return (c=='y' || c=='Y');
}

int AskIfCorrectTransaction(Transaction* t)
{
	char c='r';

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Is the following transaction correct?\n");
		t->Print();
		printf("[y/n]: ");
		scanf("%c\n",&c);
	}
	return (c=='y' || c=='Y');
}

void Finances::FindSuperAccount(const string& str,Account* a,map<string,Account*>& m,string type)
{
	char c='r';

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Does %s have a superaccount? [y/n]: ",str.c_str());
		scanf("%c\n",&c);
	}
	if(c=='n' || c=='N') return;

	printf("Please enter superaccount name. ");
	a->superaccount = ReadInAccount(m,type);
	if(a->superaccount)
		a->superaccount->subaccounts.insert(make_pair(a->name,a));
}

Account* Finances::ReadInAccount(map<string,Account*>& m,string type)
{
	char s[100];
	string str;
	Account* a;
	map<string,Account*>::iterator mit;

	while(1)
	{
		printf("%s: ",type.c_str());
		scanf("%s\n",s);
		str = s;

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
				if(AskToAdd(type,str))
				{
					a = new Account(str,type);
					m[str] = a;
					allaccounts[str] = a;

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
			return mit->second;
		}
	}
}

string ReadInInformation()
{
	char s[100];
	printf("info: ");
	scanf("%s\n",s);
	string str = s;
	return str;
}

double ReadInTotal()
{
	double d;
	printf("Amount: (positive for income or transfer, - for spent): ");
	scanf("%lf\n",&d);
	return d;
}

void Finances::ReadNewTransaction()
{
	Date* date;
	Account* tg;
	Account* l;
	Account* e;
	Account* tf;
	string info;
	double t;
	Transaction* transaction;

	date = new Date;
	date->ReadInDate();

	if(!(tg = ReadInAccount(tags,"tag"))) {delete date; return;}
	if(!(l = ReadInAccount(locations,"location"))) {delete date; return;}
	if(!(e = ReadInAccount(earmarks,"earmark"))) {delete date; return;}
	if(!(tf = ReadInAccount(tofroms,"to/from"))) {delete date; return;}

	info = ReadInInformation();
	t = ReadInTotal();

	transaction = new Transaction(date,tg,l,e,tf,info,0,t);

	if(!AskIfCorrectTransaction(transaction))
	{
		delete date;
		delete transaction;
		printf("That transaction has been discarded.\n");
		return;
	}
	else
		LinkTransaction(transaction,0);
}

void Finances::ReadNewTransfer()
{

}

void Finances::ReadNewAccount()
{

}

void LinkRecurTransaction(Transaction* t,Account* a,int multiplier)
{
	if(!a) return;
	a->amount += multiplier * t->amount;
	LinkRecurTransaction(t,a->superaccount,multiplier);
}

void LinkRecurTransfer(Transfer* t,Account* a,int multiplier)
{
	if(!a) return;
	a->amount += multiplier * t->amount;
	LinkRecurTransfer(t,a->superaccount,multiplier);
}

void Finances::LinkTransaction(Transaction* t,int loading)
{
	transactions.insert(t);

	if(!loading)
		amount += t->amount;

	t->location->transactions.insert(t);
	t->earmark->transactions.insert(t);
	t->tag->transactions.insert(t);
	t->tofrom->transactions.insert(t);

	if(!loading)
	{
		LinkRecurTransaction(t,t->location,1);
		LinkRecurTransaction(t,t->earmark,1);
		LinkRecurTransaction(t,t->tag,1);
		LinkRecurTransaction(t,t->tofrom,1);
	}


	if(!t->reconciled)
	{
		unreconciledtransactions.insert(t);
		t->location->unreconciledtransactions.insert(t);
		t->earmark->unreconciledtransactions.insert(t);
		t->tag->unreconciledtransactions.insert(t);
		t->tofrom->unreconciledtransactions.insert(t);
	}
}

void Finances::LinkTransfer(Transfer* t,int loading)
{
	transfers.insert(t);
	t->from->transfers.insert(t);
	t->to->transfers.insert(t);

	if(!loading)
	{
		LinkRecurTransfer(t,t->from,-1);
		LinkRecurTransfer(t,t->to,1);
	}

	if(!t->reconciled)
	{
		unreconciledtransfers.insert(t);
		t->from->unreconciledtransfers.insert(t);
		t->to->unreconciledtransfers.insert(t);
	}
}
