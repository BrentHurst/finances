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

	PutTransactionInTransactionVec(t,transactions);

	if(!loading)
		amount = Round2Decimals(amount + t->amount);

	PutTransactionInTransactionVec(t,t->location->transactions);
	PutTransactionInTransactionVec(t,t->earmark->transactions);
	PutTransactionInTransactionVec(t,t->tag->transactions);
	PutTransactionInTransactionVec(t,t->tofrom->transactions);

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
		PutTransactionInTransactionVec(t,unreconciledtransactions);
		PutTransactionInTransactionVec(t,t->location->unreconciledtransactions);
		PutTransactionInTransactionVec(t,t->earmark->unreconciledtransactions);
		PutTransactionInTransactionVec(t,t->tag->unreconciledtransactions);
		PutTransactionInTransactionVec(t,t->tofrom->unreconciledtransactions);
	}
}

void Finances::UnlinkTransaction(Transaction* t)
{
	RemoveTransactionFromTransactionVec(t,transactions);

	amount = Round2Decimals(amount - t->amount);

	RemoveTransactionFromTransactionVec(t,t->location->transactions);
	RemoveTransactionFromTransactionVec(t,t->earmark->transactions);
	RemoveTransactionFromTransactionVec(t,t->tag->transactions);
	RemoveTransactionFromTransactionVec(t,t->tofrom->transactions);

	LinkRecurTransaction(t,t->location,-1);
	LinkRecurTransaction(t,t->earmark,-1);
	LinkRecurTransaction(t,t->tag,-1);
	LinkRecurTransaction(t,t->tofrom,-1);

	if(!t->reconciled)
	{
		RemoveTransactionFromTransactionVec(t,unreconciledtransactions);
		RemoveTransactionFromTransactionVec(t,t->location->unreconciledtransactions);
		RemoveTransactionFromTransactionVec(t,t->earmark->unreconciledtransactions);
		RemoveTransactionFromTransactionVec(t,t->tag->unreconciledtransactions);
		RemoveTransactionFromTransactionVec(t,t->tofrom->unreconciledtransactions);
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
