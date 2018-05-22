#ifndef TRANSFER_H
#define TRANSFER_H

class Account;

#include <set>
#include "Date.h"
#include "tracomp.h"
using namespace std;


class Transfer
{
	public:
		Account* from;
		Account* to;
		Date* date;
		string info;
		int reconciled;
		double amount;
		string currency;

		int foreign;
		double foreignamount;
		string foreigncurrency;

		Transfer(Date* d,Account* f,Account* t_,
			     string& info,int r,double t,const string& curr);
		void Print();
		void Reconcile();
		Transfer* Copy();
};


typedef multiset<Transfer*,tracomp> TransferSet;

#endif
