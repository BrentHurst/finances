/*************************************************
 *   G BRENT HURST
 *   account.cpp
 *   December 27, 2017 (original)
 *   May 8, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include <cstdio>
using namespace std;



Account::Account(const string& n,AccountType ty)
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

	subaccounts.clear();
	superaccount = NULL;

	transactions.clear();
	unreconciledtransactions.clear();
	transfers.clear();
	unreconciledtransfers.clear();
}

Account::Account(const string& n,const string& ty)
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
}

void Account::Print()
{
	printf("%9.2f %s\n",amount,name.c_str());
}

void Account::Rename(const string& n)
{
	name = n;
}
