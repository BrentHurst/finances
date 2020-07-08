#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <map>
#include <set>
#include "transaction.h"
#include "transfer.h"
using namespace std;

enum AccountType {tag,location,earmark,tofrom};

typedef map<string,class Account*> AccountMap;

class Account
{
	public:
		double amount;
		string name;
		AccountType type;
		string t;
		string currency;
		int foreign;
		double foreignamount;
		string foreigncurrency;

		multimap<double,Account*> roundups;
		AccountMap subaccounts;
		Account* superaccount;

		TransactionVec transactions;
		TransactionVec unreconciledtransactions;
		TransferVec transfers;
		TransferVec unreconciledtransfers;


		//account.cpp
		Account(const string& n,AccountType ty,const string& curr);
		Account(const string& n,const string& ty,const string& curr);
		void Print(string indent);
		void PrintRoundUps();
		void Reconcile();
		void Reconcile(int i);
};


#endif
