/*************************************************
 *   G BRENT HURST
 *   finances.h
 *   December 27, 2017 (original)
 *   May 8, 2018 (last edit)
 *
 *   #include "Date.h"
 *
 *   Header file for finances program.
 *   Keeps track of physical accounts (locations),
 *   earmarks, tags, tofrom, transactions, and
 *   transfers between accounts.
 *
 ************************************************/


#include <map>
#include <set>
#include <string>
#include "Date.h"
using namespace std;


enum AccountType {tag,location,earmark,tofrom};

class Account
{
	public:
		double amount;
		string name;
		AccountType type;
		string t;

		map<string,Account*> subaccounts;
		Account* superaccount;

		multiset<class Transaction*> transactions;
		multiset<class Transaction*> unreconciledtransactions;
		multiset<class Transfer*> transfers;
		multiset<class Transfer*> unreconciledtransfers;

		Account(const string& n,AccountType ty);
		Account(const string& n,const string& ty);
		void Print();
		void Rename(const string& n);
};

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

		int operator<(const Transaction& t);
		int operator>(const Transaction& t);
		int operator<=(const Transaction& t);
		int operator>=(const Transaction& t);
		int operator==(const Transaction& t);
		int operator!=(const Transaction& t);

		Transaction(Date* d, Account* tg, Account* l,
					Account* e,Account* tf,string& i,
					int r,double t);
		void Print();
};

class Transfer
{
	public:
		Account* from;
		Account* to;
		Date* date;
		string info;
		int reconciled;
		double amount;

		int operator<(const Transfer& t);
		int operator>(const Transfer& t);
		int operator<=(const Transfer& t);
		int operator>=(const Transfer& t);
		int operator==(const Transfer& t);
		int operator!=(const Transfer& t);

		Transfer(Date* d,Account* f,Account* t_,
			     string& info,int r,double t);
		void Print();
};

class Finances
{
	protected:
		map<string,Account*> locations;
		map<string,Account*> earmarks;
		map<string,Account*> tags;
		map<string,Account*> tofroms;

		multiset<Transaction*> transactions;
		multiset<Transaction*> unreconciledtransactions;
		multiset<Transfer*> transfers;
		multiset<Transfer*> unreconciledtransfers;

		double amount;

		//loadSave.cpp
		void SaveAccounts(FILE* f);
		void SaveTransactions(FILE* f);
		void SaveTransfers(FILE* f);
		void LoadAccounts(FILE* f);
		void LoadTransactions(FILE* f);
		void LoadTransfers(FILE* f);

	public:
		//loadSave.cpp
		void Load(const string& filename);
		void Save(const string& filename);

		//finances.cpp
		//void Account* ReadInAccount();
		//void ReadNewTransaction();
		//void ReadNewTransfer();
		//void ReadNewAccount();
		Finances();
};


void PrintTransactions(const multiset<Transaction*>& transactions);
void PrintTransfers(const multiset<Transfer*>& transfers);
