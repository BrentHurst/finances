#ifndef RECONCILE_H
#define RECONCILE_H

#include "transaction.h"
#include "transfer.h"
using namespace std;

void Reconcile_(TransactionSet& s,int i);
void Reconcile_(TransferSet& s,int i);

#endif
