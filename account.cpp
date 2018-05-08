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
