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

Account* ReadInAccount(map<string,Account*>& m,string type)
{

}

//void Finances::ReadNewTransaction()
//{
	//Date* date;
	//Account* tg;
	//Account* l;
	//Account* a;
	//Account* tf;
	//string info;
	//double t;
	//Transaction* transaction;

	//date = new Date;
	//date->ReadInDate();

	//if(!(tg = ReadInAccount(tags,"tag"))) {delete date; return;}
	//if(!(l = ReadInAccount(locations,"location"))) {delete date; return;}
	//if(!(a = ReadInAccount(accounts,"account"))) {delete date; return;}
	//if(!(tf = ReadInAccount(tofroms,"to/from"))) {delete date; return;}

	//info = ReadInInformation();
	//t = ReadInTotal();

	//transaction = new Transaction(date,tg,l,a,tf,info,0,t);

	//put this in the correct maps
//}

//void Finances::ReadNewTransfer()
//{

//}

//void Finances::ReadNewAccount()
//{

//}

void Finances::LinkTransaction(Transaction* t)
{
	transactions.insert(t);
	t->location->transactions.insert(t);
	t->earmark->transactions.insert(t);
	t->tag->transactions.insert(t);
	t->tofrom->transactions.insert(t);

	if(!t->reconciled)
	{
		unreconciledtransactions.insert(t);
		t->location->unreconciledtransactions.insert(t);
		t->earmark->unreconciledtransactions.insert(t);
		t->tag->unreconciledtransactions.insert(t);
		t->tofrom->unreconciledtransactions.insert(t);
	}
}

void Finances::LinkTransfer(Transfer* t)
{
	transfers.insert(t);
	t->from->transfers.insert(t);
	t->to->transfers.insert(t);

	if(!t->reconciled)
	{
		unreconciledtransfers.insert(t);
		t->from->unreconciledtransfers.insert(t);
		t->to->unreconciledtransfers.insert(t);
	}
}
