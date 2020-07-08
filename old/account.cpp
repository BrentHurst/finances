/*************************************************
 *   G BRENT HURST
 *   account.cpp
 *   December 27, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "account.h"
#include "ask.h"
#include "number.h"
#include "reconcile.h"
#include <cstdio>
using namespace std;



Account::Account(const string& n,AccountType ty,const string& curr)
{
	amount = 0;
	name = n;
	type = ty;
	if(type==location)
		t = "location";
	else if(type==earmark)
		t = "earmark";
	else if(type==tag)
		t = "tag";
	else
		t = "tofrom";

	roundups.clear();

	subaccounts.clear();
	superaccount = NULL;

	transactions.clear();
	unreconciledtransactions.clear();
	transfers.clear();
	unreconciledtransfers.clear();

	currency = curr;

	foreign = 0;
	foreignamount = 0;
	foreigncurrency = "~-~-~";
}

Account::Account(const string& n,const string& ty,const string& curr)
{
	amount = 0;
	name = n;
	t = ty;
	if(ty=="location")
		type = location;
	else if(ty=="earmark")
		type = earmark;
	else if(ty=="tag")
		type = tag;
	else
		type = tofrom;

	subaccounts.clear();
	superaccount = NULL;

	transactions.clear();
	unreconciledtransactions.clear();
	transfers.clear();
	unreconciledtransfers.clear();

	currency = curr;

	foreign = 0;
	foreignamount = 0;
	foreigncurrency = "~-~-~";
}

void Account::Print(string indent)
{
	printf("%s%c%s%9.2f %s",indent.c_str(),(amount<0) ? '-' : (amount>0) ? '+' : ' ',currency.c_str(),abs_(amount),name.c_str());
	if(foreign)
		printf(" \t(%s%9.2f)",foreigncurrency.c_str(),foreignamount);
	printf("\n");
}

void Account::PrintRoundUps()
{
	multimap<double,Account*>::iterator mit;

	Print("");

	for(mit = roundups.begin(); mit != roundups.end(); mit++)
		printf("%f%% of round-up goes to %s.\n",100 * mit->first,mit->second->name.c_str());
}

void Account::Reconcile()
{
	Reconcile(AskReconcileDecision());
}

void Account::Reconcile(int i)
{
	Reconcile_(unreconciledtransactions,i);
	Reconcile_(unreconciledtransfers,i);
}
