/*************************************************
 *   G BRENT HURST
 *   transaction.cpp
 *   December 27, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include <cstdio>
using namespace std;


int Transaction::operator<(const Transaction& t)
{ return (*(this->date) < *(t.date)); }
int Transaction::operator>(const Transaction& t)
{ return (*(this->date) > *(t.date)); }
int Transaction::operator<=(const Transaction& t)
{ return (*(this->date) <= *(t.date)); }
int Transaction::operator>=(const Transaction& t)
{ return (*(this->date) >= *(t.date)); }
int Transaction::operator==(const Transaction& t)
{ return (*(this->date) == *(t.date)); }
int Transaction::operator!=(const Transaction& t)
{ return (*(this->date) != *(t.date)); }



Transaction::Transaction(Date* d, Account* tg, Account* l,
                         Account* a,Account* tf,string& i,
						 int r,double t)
{
	date = d;
	tag = tg;
	location = l;
	earmark = a;
	tofrom = tf;
	info = i;
	reconciled = r;
	amount = Round2Decimals(t);
}

void Transaction::Print()
{
	printf("%s %15s %15s %15s %15s %c$%9.2f %c\n\t%s\n",
			date->getDate_ddMyyyy_nothing().c_str(),
			tag->name.c_str(),
			location->name.c_str(),
			earmark->name.c_str(),
			tofrom->name.c_str(),
			(amount>=0) ? ' ' : '-',
			abs(amount),
			(reconciled) ? 'R' : '-',
			info.c_str());
}
