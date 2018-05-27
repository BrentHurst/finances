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
#include <cstdlib>
using namespace std;


Transaction::Transaction(unsigned int id_,Date* d, Account* tg, Account* l,
                         Account* a,Account* tf,string& i,
						 int r,double t,const string& curr)
{
	id = id_;
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
		printf("%u: %s %15s. %15s. %15s. %15s. %c%s%9.2f \t%c\n\t%s\n",
				id,
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
		printf("%u: %s %15s. %15s. %15s. %15s. %c%s%9.2f (%s%9.2f) \t%c\n\t%s\n",
				id,
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
	return new Transaction(id,d,tag,location,earmark,tofrom,info,reconciled,amount,currency);
}

void PutTransactionInTransactionVec(Transaction* t,TransactionVec& tv)
{
	if(tv.size() <= t->id)
		tv.resize(t->id + 1,NULL);
	else if(tv[t->id])
	{
		fprintf(stderr,"Error. tv[%u] already has something in it.\n",t->id);
		exit(0);
	}

	tv[t->id] = t;
}

void RemoveTransactionFromTransactionVec(Transaction* t,TransactionVec& tv)
{
	tv[t->id] = NULL;
}
