/*************************************************
 *   G BRENT HURST
 *   transfer.cpp
 *   December 27, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
#include <cstdio>
using namespace std;


//int Transfer::operator<(const Transfer& t)
//{ return (*(this->date) < *(t.date)); }
//int Transfer::operator>(const Transfer& t)
//{ return (*(this->date) > *(t.date)); }
//int Transfer::operator<=(const Transfer& t)
//{ return (*(this->date) <= *(t.date)); }
//int Transfer::operator>=(const Transfer& t)
//{ return (*(this->date) >= *(t.date)); }
//int Transfer::operator==(const Transfer& t)
//{ return (*(this->date) == *(t.date)); }
//int Transfer::operator!=(const Transfer& t)
//{ return (*(this->date) != *(t.date)); }


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
}

void Transfer::Print()
{
	printf("%s: %s%9.2f %15s -> %-15s \t%c\n\t%s\n",
			date->getDate_ddMyyyy_nothing().c_str(),
			currency.c_str(),
			abs_(amount),
			from->name.c_str(),
			to->name.c_str(),
			(reconciled) ? 'R' : '-',
			info.c_str());
}

void Transfer::Reconcile() { reconciled = 1; }

Transfer* Transfer::Copy()
{
	Date* d = new Date;
	d->setWithTotalDay(date->getTotalDay());
	return new Transfer(d,from,to,info,reconciled,amount,currency);
}
