/*************************************************
 *   G BRENT HURST
 *   finances.h
 *   December 27, 2017 (original)
 *   May 10, 2018 (last edit)
 *
 *   #include "Date.h"
 *
 *   Header file for finances program.
 *   Keeps track of physical accounts (locations),
 *   earmarks, tags, tofrom, transactions, and
 *   transfers between accounts.
 *
 ************************************************/

#ifndef FINANCES_H
#define FINANCES_H

#include "account.h"
#include "tracomp.h"
#include "cmdcomp.h"
#include "transfer.h"
#include "transaction.h"
#include "iodefinesandconsts.h"
#include <vector>
#include <cstdio>
#include "Date.h"
using namespace std;


class Finances
{
	public:
		AccountMap locations;
		AccountMap earmarks;
		AccountMap tags;
		AccountMap tofroms;
		AccountMap allaccounts;

		map<string,pair<double,double> > conversions;

		set<string> macronames;
		map<string,TransactionSet> macrotransactions;
		map<string,TransferVec> macrotransfers;

		TransactionSet transactions;
		TransactionSet unreconciledtransactions;
		TransferVec transfers;
		TransferVec unreconciledtransfers;
		unsigned int nexttransferid;

		string currency;

		string filename;
		string user;
		string year;

		double amount;



		//save.cpp
		void SaveAccounts(FILE* f,int newyear);
		void SaveTransactions(FILE* f,int newyear);
		void SaveTransfers(FILE* f,int newyear);
		void SaveRoundUps(FILE* f,int newyear);
		void SaveMacros(FILE* f,int newyear);
		void Save();
		void Save(const string& f,int newyear);
		void NewYear();

		//load.cpp
		void LoadAccounts(const vector<vector<string> >& file,int a,int b);
		void LoadSubaccounts(const vector<vector<string> >& file,int a,int b);
		Transaction* LoadTransaction(const vector<vector<string> >& file,int i);
		void LoadTransactions(const vector<vector<string> >& file,int a,int b);
		Transfer* LoadTransfer(const vector<vector<string> >& file,int i);
		void LoadTransfers(const vector<vector<string> >& file,int a,int b);
		void LoadRoundUps(const vector<vector<string> >& file,int a,int b);
		void LoadMacros(const vector<vector<string> >& file,int a,int b);
		void LoadCheck();
		string GetFileName();
		void Load();

		//printfinances.cpp
		void PrintTransactions();
		void PrintTransfers();
		void PrintUnreconciledTransactions();
		void PrintUnreconciledTransfers();
		void PrintAccountRecur(Account* a,string indent);
		void PrintEarmarks();
		void PrintLocations();
		void PrintTags();
		void PrintTofroms();
		void PrintAllAccounts();
		void PrintCorrectAccountMap(const string& type);
		void PrintForeignTransactions();
		void PrintForeignTransfers();
		void PrintMacros();

		//link.cpp
		void LinkTransaction(Transaction* t,int loading);
		void LinkTransfer(Transfer* t,int loading);
		void UnlinkTransaction(Transaction* t);
		void UnlinkTransfer(Transfer* t);
		void LinkRecurTransaction(Transaction* t,Account* a,int multiplier);
		void LinkRecurTransfer(Transfer* t,Account* a,int multiplier);

		//reconcilefinances.cpp
		void Reconcile();

		//merge.cpp
		void Merge(Transfer* t);

		//selecttransaction.cpp
		void SelectTransaction(TransactionSet& ts);

		//selecttransfer.cpp
		void SelectTransfer(TransferVec& ts);

		//macro.cpp
		void Macros();
		void RunAMacro();
		void AddAMacro();
		void DeleteAMacro();

		//selectaccount.cpp
		Account* GetAccountFromUser(AccountMap& m);
		void SelectAccount();
		void AddRoundUp(Account* a);
		void GiveAnotherSuperaccount(Account* a);

		//setup.cpp
		void SetupAddAccounts(const string& type,AccountMap& m);
		void Setup();

		//readtra.cpp
		Transaction* ReadNewTransaction(int link,int fgn);
		Transfer* ReadNewTransfer(int link,int fgn);
		void ReadNewForeignTransaction();
		void ReadTransferToForeign();
		void ReadTransferFromForeign();

		//readaccount.cpp
		void FindSuperAccount(const string& str,Account* a,AccountMap& m,string type,int setup);
		void ReadNewAccount();
		Account* ReadInAccount(AccountMap& m,string type,int z,int setup);
		void RenameAccount(Account* a);

		//finances.cpp
		Finances();
};

#endif
