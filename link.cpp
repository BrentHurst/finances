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
#include "number.h"
using namespace std;


void Finances::LinkRecurTransaction(Transaction* t,Account* a,int multiplier)
{
	if(!a) return;
	a->amount = Round2Decimals(a->amount + multiplier * t->amount);
	if(t->foreign)
		a->foreignamount = Round2Decimals(a->foreignamount + multiplier * t->foreignamount);
	LinkRecurTransaction(t,a->superaccount,multiplier);
}

void Finances::LinkRecurTransfer(Transfer* t,Account* a,int multiplier)
{
	if(!a) return;
	a->amount = Round2Decimals(a->amount + multiplier * t->amount);
	if(t->foreign)
		a->foreignamount = Round2Decimals(a->foreignamount + multiplier * t->foreignamount);
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
			transfer = new Transfer(
									nexttransferid,
									d,
									t->location,
									mit->second,
									s,
									0,
									Round2Decimals(mit->first * FindRoundUpAmount(t->amount)),
									currency
									);
			nexttransferid++;
			LinkTransfer(transfer,0);
		}
	}

	if(!loading && Round2Decimals(FindRoundUpAmount(t->amount)) && !t->earmark->roundups.empty())
	{
		for(mit = t->earmark->roundups.begin(); mit != t->earmark->roundups.end();  mit++)
		{
			d = new Date;
			d->setWithTotalDay(t->date->getTotalDay());
			transfer = new Transfer(
									nexttransferid,
									d,
									t->earmark,
									mit->second,
									s,
									0,
									Round2Decimals(mit->first * FindRoundUpAmount(t->amount)),
									currency
									);
			nexttransferid++;
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
	PutTransferInTransferVec(t,transfers);
	PutTransferInTransferVec(t,t->from->transfers);
	PutTransferInTransferVec(t,t->to->transfers);

	if(!loading)
	{
		LinkRecurTransfer(t,t->from,-1);
		LinkRecurTransfer(t,t->to,1);
	}

	if(!t->reconciled)
	{
		PutTransferInTransferVec(t,unreconciledtransfers);
		PutTransferInTransferVec(t,t->from->unreconciledtransfers);
		PutTransferInTransferVec(t,t->to->unreconciledtransfers);
	}
}

void Finances::UnlinkTransfer(Transfer* t)
{
	RemoveTransferFromTransferVec(t,transfers);
	RemoveTransferFromTransferVec(t,t->from->transfers);
	RemoveTransferFromTransferVec(t,t->to->transfers);

	LinkRecurTransfer(t,t->from,1);
	LinkRecurTransfer(t,t->to,-1);

	if(!t->reconciled)
	{
		RemoveTransferFromTransferVec(t,unreconciledtransfers);
		RemoveTransferFromTransferVec(t,t->from->unreconciledtransfers);
		RemoveTransferFromTransferVec(t,t->to->unreconciledtransfers);
	}
}
