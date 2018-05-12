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

const char c0 = ',';
const int c1 = '\n';
const int esc = '`';


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

		//account.cpp
		Account(const string& n,AccountType ty);
		Account(const string& n,const string& ty);

		void Print(string indent);
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

		//transaction.cpp
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

		//transfer.cpp
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
		map<string,Account*> allaccounts;

		map<string,Account*> locations;
		map<string,Account*> earmarks;
		map<string,Account*> tags;
		map<string,Account*> tofroms;

		multiset<Transaction*> transactions;
		multiset<Transaction*> unreconciledtransactions;
		multiset<Transfer*> transfers;
		multiset<Transfer*> unreconciledtransfers;

		double amount;

		//save.cpp
		void SaveAccounts(FILE* f);
		void SaveTransactions(FILE* f);
		void SaveTransfers(FILE* f);

		//load.cpp
		void LoadAccounts(const vector<vector<string> >& file,int a,int b);
		void LoadSubaccounts(const vector<vector<string> >& file,int a,int b);
		void LoadTransactions(const vector<vector<string> >& file,int a,int b);
		void LoadTransfers(const vector<vector<string> >& file,int a,int b);

		//link.cpp
		void LinkRecurTransaction(Transaction* t,Account* a,int multiplier);
		void LinkRecurTransfer(Transfer* t,Account* a,int multiplier);
		void LinkTransaction(Transaction* t,int loading);
		void LinkTransfer(Transfer* t,int loading);

		//finances.cpp
		void FindSuperAccount(const string& str,Account* a,map<string,Account*>& m,string type,int setup);
		Account* ReadInAccount(map<string,Account*>& m,string type,int z,int setup);

		//print.cpp
		void PrintAccountRecur(Account* a,string indent);

		//setup.cpp
		void SetupAddAccounts(const string& type,map<string,Account*>& m);

	public:
		//load.cpp
		void Load(const string& filename);

		//save.cpp
		void Save(const string& filename);

		//finances.cpp
		void ReadNewTransaction();
		void ReadNewTransfer();
		void ReadNewAccount();
		Finances();

		//setup.cpp
		void Setup(const string& filename);

		//print.cpp
		void PrintTransactions();
		void PrintTransfers();
		void PrintEarmarks();
		void PrintLocations();
		void PrintTags();
		void PrintTofroms();
		void PrintAllAccounts();
};


//global.cpp
void PrintTransactionsGlobal(const multiset<Transaction*>& transactions);
void PrintTransfersGlobal(const multiset<Transfer*>& transfers);
string dtos_(double d);
string itos_(int i);
double stod_(string s);
int stoi_(string s);
double abs(double f);
double Round2Decimals(double d);
string ReadString();


//ask.cpp
int AskForContinue();
int AskToAdd(const string& type,const string& str);
int AskIfCorrectTransaction(Transaction* t);
int AskIfCorrectTransfer(Transfer* t);
