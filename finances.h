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

#include <map>
#include <set>
#include <string>
#include <vector>
#include "Date.h"
using namespace std;

#define PutLine PutDelimitedLine(f,c0,c1,esc,v)
#define PutLines PutDelimitedFile(f,c0,c1,esc,subaccounts)
#define GetLine GetDelimitedLine(f,c0,c1,esc,v)
#define GetLines GetDelimitedFile(f,c0,c1,esc,file)
#define FlushInputBuffer while((junk=getchar()) != '\n' && junk != EOF)

//#define TransactionSet multiset<Transaction*,tracomp<Transaction*> >
//#define TransferSet multiset<Transfer*,tracomp<Transfer*> >
#define TransactionSet multiset<Transaction*>
#define TransferSet multiset<Transfer*>

const char c0 = '\0';
const int c1 = '\n';
const int esc = '`';

class Transaction;
class Transfer;

template <class T>
class tracomp
{
	public:
		bool operator() (T const& a,T const& b);
};


enum AccountType {tag,location,earmark,tofrom};

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
		map<string,Account*> subaccounts;
		Account* superaccount;

		TransactionSet transactions;
		TransactionSet unreconciledtransactions;
		TransferSet transfers;
		TransferSet unreconciledtransfers;

		//account.cpp
		Account(const string& n,AccountType ty,const string& curr);
		Account(const string& n,const string& ty,const string& curr);
		void Print(string indent);
		void PrintRoundUps();

		//reconcile.cpp
		void Reconcile();
		void Reconcile(int i);
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
		string currency;
		int foreign;
		double foreignamount;
		string foreigncurrency;

		//transaction.cpp
		//int operator<(const Transaction& t);
		//int operator>(const Transaction& t);
		//int operator<=(const Transaction& t);
		//int operator>=(const Transaction& t);
		//int operator==(const Transaction& t);
		//int operator!=(const Transaction& t);

		Transaction(Date* d, Account* tg, Account* l,
					Account* e,Account* tf,string& i,
					int r,double t,const string& curr);
		void Print();
		void Reconcile();
		Transaction* Copy();
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
		string currency;

		int foreign;
		int foreignamount;
		int foreigncurrency;

		//transfer.cpp
		//int operator<(const Transfer t);
		//int operator>(const Transfer t);
		//int operator<=(const Transfer t);
		//int operator>=(const Transfer t);
		//int operator==(const Transfer t);
		//int operator!=(const Transfer t);

		Transfer(Date* d,Account* f,Account* t_,
			     string& info,int r,double t,const string& curr);
		void Print();
		void Reconcile();
		Transfer* Copy();
};


class Finances
{
	protected:
		map<string,Account*> allaccounts;

		double amount;

		//save.cpp
		void SaveAccounts(FILE* f,int newyear);
		void SaveTransactions(FILE* f,int newyear);
		void SaveTransfers(FILE* f,int newyear);
		void SaveRoundUps(FILE* f,int newyear);
		void SaveMacros(FILE* f,int newyear);

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

		//link.cpp
		void LinkRecurTransaction(Transaction* t,Account* a,int multiplier);
		void LinkRecurTransfer(Transfer* t,Account* a,int multiplier);

		//finances.cpp
		void FindSuperAccount(const string& str,Account* a,map<string,Account*>& m,string type,int setup);

		//print.cpp
		void PrintAccountRecur(Account* a,string indent);

		//setup.cpp
		void SetupAddAccounts(const string& type,map<string,Account*>& m);

	public:
		map<string,Account*> locations;
		map<string,Account*> earmarks;
		map<string,Account*> tags;
		map<string,Account*> tofroms;

		map<string,pair<double,double> > conversions;

		set<string> macronames;
		map<string,TransactionSet> macrotransactions;
		map<string,TransferSet> macrotransfers;

		TransactionSet transactions;
		TransactionSet unreconciledtransactions;
		TransferSet transfers;
		TransferSet unreconciledtransfers;

		string currency;

		string filename;
		string user;
		string year;


		//load.cpp
		void Load();

		//save.cpp
		void Save();
		void Save(const string& f,int newyear);
		void NewYear();

		//link.cpp
		void LinkTransaction(Transaction* t,int loading);
		void LinkTransfer(Transfer* t,int loading);
		void UnlinkTransaction(Transaction* t);
		void UnlinkTransfer(Transfer* t);

		//finances.cpp
		Transaction* ReadNewTransaction(int link,int fgn);
		Transfer* ReadNewTransfer(int link,int fgn);
		void ReadNewAccount();
		Finances();
		void RenameAccount(Account* a);
		Account* ReadInAccount(map<string,Account*>& m,string type,int z,int setup);

		//reconcile.cpp
		void Reconcile();

		//setup.cpp
		void Setup();

		//print.cpp
		void PrintTransactions();
		void PrintTransfers();
		void PrintUnreconciledTransactions();
		void PrintUnreconciledTransfers();
		void PrintEarmarks();
		void PrintLocations();
		void PrintTags();
		void PrintTofroms();
		void PrintAllAccounts();
		void PrintCorrectAccountMap(const string& type);

		//selectaccount.cpp
		Account* GetAccountFromUser(map<string,Account*>& m);
		void SelectAccount();
		void AddRoundUp(Account* a);
		void GiveAnotherSuperaccount(Account* a);

		//selecttra.cpp
		void SelectTransaction(TransactionSet& ts);
		void SelectTransfer(TransferSet& ts);

		//merge.cpp
		void Merge(Transfer* t);

		//macros.cpp
		void Macros();
		void PrintMacros();
		void RunAMacro();
		void AddAMacro();
		void DeleteAMacro();

		//foreign.cpp
		void ReadNewForeignTransaction();
		void ReadTransferToForeign();
		void ReadTransferFromForeign();
		void PrintForeignTransactions();
		void PrintForeignTransfers();
};


//global.cpp
void PrintTransactionsGlobal(const TransactionSet& transactions);
void PrintTransfersGlobal(const TransferSet& transfers);
string dtos_(double d);
string itos_(int i);
double stod_(string s);
int stoi_(string s);
double abs_(double f);
double Round2Decimals(double d);
string ReadString();
char ReadChar();
double ReadDouble();
int ReadInt();
char GetCommand(map<char,string>& cmdList);
void PrintCommands(map<char,string>& cmdList);
string ReadInInformation();
double ReadInTotal();
double FindRoundUpAmount(double d);

//reconcile.cpp
void Reconcile_(TransactionSet& s,int i);
void Reconcile_(TransferSet& s,int i);

//ask.cpp
int AskForContinue();
int AskToAdd(const string& type,const string& str);
int AskIfCorrectTransaction(Transaction* t);
int AskIfCorrectTransfer(Transfer* t);

#endif
