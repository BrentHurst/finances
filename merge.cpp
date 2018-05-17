/*************************************************
 *   G BRENT HURST
 *   merge.cpp
 *   May 16, 2018 (original)
 *   May 16, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
#include <cstdio>
using namespace std;

void Finances::Merge(Transfer* t)
{
	char c;
	Transfer* t2;
	TransferSet::iterator sit;

	Date* d;
	Account* from;
	Account* to;
	string info;
	int reconciled;
	double amount;


	do
	{
		printf("Merge all reconciled transfers that are \"%s  ->  %s\"? [y/n]: ",t->from->name.c_str(),t->to->name.c_str());
		c = ReadChar();
	}while(c!='y' && c!='Y' && c!='n' && c!='N');

	if(c=='n' || c=='N')
		return;

	d = new Date;
	d->setWithTotalDay(t->date->getTotalDay());
	from = t->from;
	to = t->to;
	info = "Merged";
	reconciled = 1;
	amount = 0;

	for(sit=transfers.begin(); sit != transfers.end(); )
	{
		t2 = *sit;
		sit++;
		if(t2->reconciled && t2->from==from && t2->to==to)
		{
			amount = Round2Decimals(amount + t2->amount);
			UnlinkTransfer(t2);
			delete t2->date;
			delete t2;
		}
	}

	t2 = new Transfer(d,from,to,info,reconciled,amount);
	LinkTransfer(t2,0);

	printf("Transfers Successfully Merged\n");

	t = NULL;
}
