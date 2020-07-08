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
#include "number.h"
#include "readtype.h"
#include <cstdio>
using namespace std;

void Finances::Merge(Transfer* t)
{
	char c;
	Transfer* t2;
	unsigned int i;

	int id_;
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
	id_ = t->id;
	from = t->from;
	to = t->to;
	info = "Merged";
	reconciled = 1;
	amount = 0;

	for(i=0; i<transfers.size(); i++)
		if(transfers[i] && transfers[i]->reconciled && transfers[i]->from==from && transfers[i]->to==to && (transfers[i]->info=="Automatic Round-Up Transfer" || transfers[i]->info=="Merged"))
		{
			t2 = transfers[i];
			amount = Round2Decimals(amount + t2->amount);
			UnlinkTransfer(t2);
			delete t2->date;
			delete t2;
		}

	t2 = new Transfer(id_,d,from,to,info,reconciled,amount,currency);
	LinkTransfer(t2,0);

	printf("Transfers Successfully Merged\n");

	t = NULL;
}
