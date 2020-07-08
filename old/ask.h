#ifndef ASK_H
#define ASK_H

#include "transaction.h"
#include "transfer.h"
#include <string>
using namespace std;


int AskForContinue();
int AskToAdd(const string& type,const string& str);
int AskIfCorrectTransaction(Transaction* t);
int AskIfCorrectTransfer(Transfer* t);
int AskReconcileDecision();

#endif
