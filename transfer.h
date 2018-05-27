#ifndef TRANSFER_H
#define TRANSFER_H

class Account;

#include <vector>
#include "Date.h"
using namespace std;


class Transfer
{
	public:
		unsigned int id;
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

		Transfer(int id_,Date* d,Account* f,Account* t_,
			     string& info,int r,double t,const string& curr);
		void Print();
		void Reconcile();
		Transfer* Copy();
};

typedef vector<Transfer*> TransferVec;

void PutTransferInTransferVec(Transfer* t,TransferVec& tv);
void RemoveTransferFromTransferVec(Transfer* t,TransferVec& tv);
void InsertionSort3(TransferVec& tv);

#endif
