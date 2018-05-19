/*************************************************
 *   G BRENT HURST
 *   link.cpp
 *   May 11, 2018 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
using namespace std;


void Finances::LinkRecurTransaction(Transaction* t,Account* a,int multiplier)
{
	if(!a) return;
	a->amount = Round2Decimals(a->amount + multiplier * t->amount);
	LinkRecurTransaction(t,a->superaccount,multiplier);
}

void Finances::LinkRecurTransfer(Transfer* t,Account* a,int multiplier)
{
	if(!a) return;
	a->amount = Round2Decimals(a->amount + multiplier * t->amount);
	LinkRecurTransfer(t,a->superaccount,multiplier);
}

void Finances::LinkTransaction(Transaction* t,int loading)
{
	Transfer* transfer;
	Date* d;
	multimap<double,Account*>::iterator mit;
	string s = "Automatic Round-Up Transfer";

	transactions.insert(t);

	if(!loading)
		amount = Round2Decimals(amount + t->amount);

	t->location->transactions.insert(t);
	t->earmark->transactions.insert(t);
	t->tag->transactions.insert(t);
	t->tofrom->transactions.insert(t);

	if(!loading && Round2Decimals(FindRoundUpAmount(t->amount)) && !t->location->roundups.empty())
	{
		for(mit = t->location->roundups.begin(); mit != t->location->roundups.end();  mit++)
		{
			d = new Date;
			d->setWithTotalDay(t->date->getTotalDay());
			transfer = new Transfer(d,
									t->location,
									mit->second,
									s,
									0,
									Round2Decimals(mit->first * FindRoundUpAmount(t->amount)),
									currency
									);
			LinkTransfer(transfer,0);
		}
	}

	if(!loading && Round2Decimals(FindRoundUpAmount(t->amount)) && !t->earmark->roundups.empty())
	{
		for(mit = t->earmark->roundups.begin(); mit != t->earmark->roundups.end();  mit++)
		{
			d = new Date;
			d->setWithTotalDay(t->date->getTotalDay());
			transfer = new Transfer(d,
									t->earmark,
									mit->second,
									s,
									0,
									Round2Decimals(mit->first * FindRoundUpAmount(t->amount)),
									currency
									);
			LinkTransfer(transfer,0);
		}
	}

	if(!loading)
	{
		LinkRecurTransaction(t,t->location,1);
		LinkRecurTransaction(t,t->earmark,1);
		LinkRecurTransaction(t,t->tag,1);
		LinkRecurTransaction(t,t->tofrom,1);
	}


	if(!t->reconciled)
	{
		unreconciledtransactions.insert(t);
		t->location->unreconciledtransactions.insert(t);
		t->earmark->unreconciledtransactions.insert(t);
		t->tag->unreconciledtransactions.insert(t);
		t->tofrom->unreconciledtransactions.insert(t);
	}
}

void Finances::UnlinkTransaction(Transaction* t)
{
	transactions.erase(transactions.find(t));

	amount = Round2Decimals(amount - t->amount);

	t->location->transactions.erase(t->location->transactions.find(t));
	t->earmark->transactions.erase(t->earmark->transactions.find(t));
	t->tag->transactions.erase(t->tag->transactions.find(t));
	t->tofrom->transactions.erase(t->tofrom->transactions.find(t));

	LinkRecurTransaction(t,t->location,-1);
	LinkRecurTransaction(t,t->earmark,-1);
	LinkRecurTransaction(t,t->tag,-1);
	LinkRecurTransaction(t,t->tofrom,-1);

	if(!t->reconciled)
	{
		unreconciledtransactions.erase(unreconciledtransactions.find(t));
		t->location->unreconciledtransactions.erase(t->location->unreconciledtransactions.find(t));
		t->earmark->unreconciledtransactions.erase(t->earmark->unreconciledtransactions.find(t));
		t->tag->unreconciledtransactions.erase(t->tag->unreconciledtransactions.find(t));
		t->tofrom->unreconciledtransactions.erase(t->tofrom->unreconciledtransactions.find(t));
	}
}

void Finances::LinkTransfer(Transfer* t,int loading)
{
	transfers.insert(t);
	t->from->transfers.insert(t);
	t->to->transfers.insert(t);

	if(!loading)
	{
		LinkRecurTransfer(t,t->from,-1);
		LinkRecurTransfer(t,t->to,1);
	}

	if(!t->reconciled)
	{
		unreconciledtransfers.insert(t);
		t->from->unreconciledtransfers.insert(t);
		t->to->unreconciledtransfers.insert(t);
	}
}

void Finances::UnlinkTransfer(Transfer* t)
{
	transfers.erase(transfers.find(t));

	t->from->transfers.erase(t->from->transfers.find(t));
	t->to->transfers.erase(t->to->transfers.find(t));

	LinkRecurTransfer(t,t->from,1);
	LinkRecurTransfer(t,t->to,-1);

	if(!t->reconciled)
	{
		unreconciledtransfers.erase(unreconciledtransfers.find(t));
		t->from->unreconciledtransfers.erase(t->from->unreconciledtransfers.find(t));
		t->to->unreconciledtransfers.erase(t->to->unreconciledtransfers.find(t));
	}
}

