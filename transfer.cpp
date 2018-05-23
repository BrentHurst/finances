/*************************************************
 *   G BRENT HURST
 *   transfer.cpp
 *   December 27, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "transfer.h"
#include "account.h"
#include "number.h"
#include <cstdio>
using namespace std;


Transfer::Transfer(Date* d,Account* f,Account* t_,
		           string& i,int r,double t,const string& curr)
{
	date = d;
	from = f;
	to = t_;
	info = i;
	reconciled = r;
	amount = Round2Decimals(t);
	currency = curr;

	//foreign = 0;
	//foreignamount = 0;
}

void Transfer::Print()
{
	if(!foreign)
		printf("%s: %s%9.2f %15s -> %-15s \t%c\n\t%s\n",
				date->getDate_ddMyyyy_nothing().c_str(),
				currency.c_str(),
				abs_(amount),
				from->name.c_str(),
				to->name.c_str(),
				(reconciled) ? 'R' : '-',
				info.c_str()
			  );
	else
		printf("%s: %s%9.2f (%s%9.2f) %15s -> %-15s \t%c\n\t%s\n",
				date->getDate_ddMyyyy_nothing().c_str(),
				currency.c_str(),
				abs_(amount),
				foreigncurrency.c_str(),
				abs_(foreignamount),
				from->name.c_str(),
				to->name.c_str(),
				(reconciled) ? 'R' : '-',
				info.c_str()
			  );
}

void Transfer::Reconcile() { reconciled = 1; }

Transfer* Transfer::Copy()
{
	Date* d = new Date;
	d->setWithTotalDay(date->getTotalDay());
	return new Transfer(d,from,to,info,reconciled,amount,currency);
}
