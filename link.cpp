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
	a->amount += multiplier * t->amount;
	LinkRecurTransaction(t,a->superaccount,multiplier);
}

void Finances::LinkRecurTransfer(Transfer* t,Account* a,int multiplier)
{
	if(!a) return;
	a->amount += multiplier * t->amount;
	LinkRecurTransfer(t,a->superaccount,multiplier);
}

void Finances::LinkTransaction(Transaction* t,int loading)
{
	transactions.insert(t);

	if(!loading)
		amount += t->amount;

	t->location->transactions.insert(t);
	t->earmark->transactions.insert(t);
	t->tag->transactions.insert(t);
	t->tofrom->transactions.insert(t);

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

