#include "finances.h"
#include "readtype.h"
#include "number.h"
#include "ask.h"
using namespace std;

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

//fgn = 1 if toforeign
//fgn = 2 if fromforeign
Transfer* Finances::ReadNewTransfer(int link,int fgn)
{
	Date* date;
	Account* from;
	Account* to;
	string info;
	double amount;
	Transfer* t;
	char type='r';
	string curr;
	double d;


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

	info = ReadInInformation();
	if(fgn==1)
		printf("Enter in %s\n",currency.c_str());
	else if(fgn==2)
		printf("Enter in foreign currency\n");
	amount = Round2Decimals(ReadInTotal());

	t = new Transfer(date,from,to,info,0,amount,currency);

	if(fgn)
	{
		if(fgn==1 && (!to->foreign || from->foreign))
		{
			printf("'to' should be foreign and 'from' should not.\n");
			delete date;
			return NULL;
		}
		else if(fgn==2 && (to->foreign || !from->foreign))
		{
			printf("'from' should be foreign and 'to' should not.\n");
			delete date;
			return NULL;
		}

		curr = (fgn==1) ? to->foreigncurrency : from->foreigncurrency;

		t->foreign = fgn;
		t->foreigncurrency = curr;

		if(fgn==1)
		{
			printf("How many %s is this? %s",curr.c_str(),curr.c_str());
			d = ReadDouble();
			t->foreignamount = d;
			if(link)
			{
				conversions[curr].first += t->amount;
				conversions[curr].second += Round2Decimals(d);
			}
		}
		else
		{
			printf("How many %s is this? %s",currency.c_str(),currency.c_str());
			d = ReadDouble();
			t->foreignamount = t->amount;
			t->amount = d;
			if(link)
			{
				conversions[curr].first -= Round2Decimals(t->amount);
				conversions[curr].second -= Round2Decimals(t->foreignamount);
			}
		}
	}

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

void Finances::ReadNewForeignTransaction()
{
	ReadNewTransaction(1,1);
}

void Finances::ReadTransferToForeign()
{
	ReadNewTransfer(1,1);
}

void Finances::ReadTransferFromForeign()
{
	ReadNewTransfer(1,2);
}
