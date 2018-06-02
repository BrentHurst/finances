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
#include <cstdlib>
#include <cstdio>
using namespace std;


Transfer::Transfer(int id_,Date* d,Account* f,Account* t_,
		           string& i,int r,double t,const string& curr)
{
	id = id_;
	date = d;
	from = f;
	to = t_;
	info = i;
	reconciled = r;
	amount = Round2Decimals(t);
	currency = curr;

	foreign = 0;
	foreignamount = 0;
}

void Transfer::Print()
{
	if(!foreign)
		printf("%d: %s %s%9.2f %15s -> %-15s \t%c\n\t%s\n",
				id,
				date->getDate_ddMyyyy_nothing().c_str(),
				currency.c_str(),
				abs_(amount),
				from->name.c_str(),
				to->name.c_str(),
				(reconciled) ? 'R' : '-',
				info.c_str()
			  );
	else
		printf("%d: %s %s%9.2f (%s%9.2f) %15s -> %-15s \t%c\n\t%s\n",
				id,
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
	return new Transfer(id,d,from,to,info,reconciled,amount,currency);
}

void PutTransferInTransferVec(Transfer* t,TransferVec& tv)
{
	if(tv.size() <= t->id)
		tv.resize(t->id + 1,NULL);
	else if(tv[t->id])
	{
		fprintf(stderr,"Error. tv[%d] already has something in it.\n",t->id);
		exit(0);
	}

	tv[t->id] = t;
}

void RemoveTransferFromTransferVec(Transfer* t,TransferVec& tv)
{
	tv[t->id] = NULL;
}

static int Less(Transfer* a,Transfer* b)
{
	return (*(a->date) < *(b->date));
}

void InsertionSort3(TransferVec& tv)
{
	unsigned int i;
	unsigned int j;
	unsigned int size;
	unsigned int minindex;
	Transfer* tmp;

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
		for(j = i; Less(tmp,tv[j-1]); j--)
			tv[j] = tv[j-1];
		tv[j] = tmp;
	}
}
