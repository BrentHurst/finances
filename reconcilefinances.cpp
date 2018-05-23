/*************************************************
 *   G BRENT HURST
 *   reconcile.cpp
 *   May 13, 2018 (original)
 *   May 13, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include <set>
#include <cstdio>
#include "finances.h"
using namespace std;

void Finances::Reconcile()
{
	int i = AskReconcileDecision();
	Reconcile_(unreconciledtransactions,i);
	Reconcile_(unreconciledtransfers,i);
}
