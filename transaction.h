#ifndef TRANSACTION_H
#define TRANSACTION_H

class Account;

#include "Date.h"
#include <vector>
using namespace std;


class Transaction
{
	public:
		unsigned int id;
		Date* date;
		Account* tag;
		Account* location;
		Account* earmark;
		Account* tofrom;
		string info;
		int reconciled;
		double amount;
		string currency;
		int foreign;
		double foreignamount;
		string foreigncurrency;

		Transaction(unsigned int id_,Date* d, Account* tg, Account* l,
					Account* e,Account* tf,string& i,
					int r,double t,const string& curr);
		void Print();
		void Reconcile();
		Transaction* Copy();
};

typedef vector<Transaction*> TransactionVec;

void PutTransactionInTransactionVec(Transaction* t,TransactionVec& tv);
void RemoveTransactionFromTransactionVec(Transaction* t,TransactionVec& tv);

void InsertionSort3(TransactionVec& tv);

#endif
