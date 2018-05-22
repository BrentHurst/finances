#ifndef RECONCILE_H
#define RECONCILE_H

using namespace std;
#include "transaction.h"
#include "transfer.h"

void Reconcile_(TransactionSet& s,int i);
void Reconcile_(TransferSet& s,int i);

#endif
