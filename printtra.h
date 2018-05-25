#ifndef PRINTTRA_H
#define PRINTTRA_H

#include "transaction.h"
#include "transfer.h"
using namespace std;

void PrintTransactionsGlobal(const TransactionSet& transactions);
void PrintTransfersGlobal(const TransferVec& transfers);

#endif
