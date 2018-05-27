/*************************************************
 *   G BRENT HURST
 *   transaction.cpp
 *   December 27, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "transaction.h"
#include "account.h"
#include "number.h"
#include <cstdio>
using namespace std;


Transaction::Transaction(Date* d, Account* tg, Account* l,
                         Account* a,Account* tf,string& i,
						 int r,double t,const string& curr)
{
	date = d;
	tag = tg;
	location = l;
	earmark = a;
	tofrom = tf;
	info = i;
	reconciled = r;
	amount = Round2Decimals(t);
	currency = curr;

	foreign = 0;
	foreignamount = 0;
	foreigncurrency = "~-~-~";
}

void Transaction::Print()
{
	if(!foreign)
		printf("%s %15s. %15s. %15s. %15s. %c%s%9.2f \t%c\n\t%s\n",
				date->getDate_ddMyyyy_nothing().c_str(),
				tag->name.c_str(),
				location->name.c_str(),
				earmark->name.c_str(),
				tofrom->name.c_str(),
				(amount>=0) ? ' ' : '-',
				currency.c_str(),
				abs_(amount),
				(reconciled) ? 'R' : '-',
				info.c_str());
	else //if foreign
		printf("%s %15s. %15s. %15s. %15s. %c%s%9.2f (%s%9.2f) \t%c\n\t%s\n",
				date->getDate_ddMyyyy_nothing().c_str(),
				tag->name.c_str(),
				location->name.c_str(),
				earmark->name.c_str(),
				tofrom->name.c_str(),
				(amount>=0) ? ' ' : '-',
				currency.c_str(),
				abs_(amount),
				foreigncurrency.c_str(),
				foreignamount,
				(reconciled) ? 'R' : '-',
				info.c_str());
}

void Transaction::Reconcile() { reconciled = 1; }

Transaction* Transaction::Copy()
{
	Date* d = new Date;
	d->setWithTotalDay(date->getTotalDay());
	return new Transaction(d,tag,location,earmark,tofrom,info,reconciled,amount,currency);
}

static int Less(Transaction* a,Transaction* b)
{
	return (*(a->date) < *(b->date));
}

void InsertionSort3(TransactionVec& tv)
{
	unsigned int i;
	unsigned int j;
	unsigned int size;
	unsigned int minindex;
	Transaction* tmp;

	size = tv.size();

	for(i=0; i<size; i++)
		if(!tv[i])
			return;

	minindex = 0;
	for(i=1; i<size; i++)
		if(Less(tv[i],tv[minindex]))
			minindex = i;
	tmp = tv[0];
	tv[0] = tv[minindex];
	tv[minindex] = tmp;

	for(i=1; i<size; i++)
	{
		tmp = tv[i];
		for(j = i; tmp < tv[j-1]; j--)
			tv[j] = tv[j-1];
		tv[j] = tmp;
	}
}
