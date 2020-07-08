#ifndef PRINTTRA_H
#define PRINTTRA_H

#include "transaction.h"
#include "transfer.h"
using namespace std;

void PrintTransactionsGlobal(const TransactionVec& transactions);
void PrintTransfersGlobal(const TransferVec& transfers);

#endif
