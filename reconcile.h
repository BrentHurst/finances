#ifndef RECONCILE_H
#define RECONCILE_H

#include "transaction.h"
#include "transfer.h"
using namespace std;

void Reconcile_(TransactionVec& s,int i);
void Reconcile_(TransferVec& s,int i);

#endif
