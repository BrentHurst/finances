#ifndef TRANSACTION_H
#define TRANSACTION_H

class Account;

#include "Date.h"
#include <set>
#include "tracomp.h"
using namespace std;


class Transaction
{
	public:
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

		Transaction(Date* d, Account* tg, Account* l,
					Account* e,Account* tf,string& i,
					int r,double t,const string& curr);
		void Print();
		void Reconcile();
		Transaction* Copy();
};

typedef multiset<Transaction*,tracomp> TransactionSet;

#endif
