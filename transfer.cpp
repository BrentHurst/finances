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


int Transfer::operator<(const Transfer& t)
{ return (*(this->date) < *(t.date)); }
int Transfer::operator>(const Transfer& t)
{ return (*(this->date) > *(t.date)); }
int Transfer::operator<=(const Transfer& t)
{ return (*(this->date) <= *(t.date)); }
int Transfer::operator>=(const Transfer& t)
{ return (*(this->date) >= *(t.date)); }
int Transfer::operator==(const Transfer& t)
{ return (*(this->date) == *(t.date)); }
int Transfer::operator!=(const Transfer& t)
{ return (*(this->date) != *(t.date)); }


Transfer::Transfer(Date* d,Account* f,Account* t_,
		           string& i,int r,double t)
{
	date = d;
	from = f;
	to = t_;
	info = i;
	reconciled = r;
	amount = Round2Decimals(t);
}

void Transfer::Print()
{
	printf("%s: $%9.2f %15s -> %-15s \t%c\n\t%s\n",
			date->getDate_ddMyyyy_nothing().c_str(),
			abs(amount),
			from->name.c_str(),
			to->name.c_str(),
			(reconciled) ? 'R' : '-',
			info.c_str());
}

void Transfer::Reconcile() { reconciled = 1; }
