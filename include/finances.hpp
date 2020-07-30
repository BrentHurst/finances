#ifndef FINANCES_H
#define FINANCES_H

#include <map>
#include <string>
#include <vector>
#include "utilities.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using nlohmann::json;

const string ErrorAsterisks = "*!*!*!*!*!*";
const string DefaultPrompt = "~~~~>";

// TODO - Write some checks for after reading, including to require HeadAccounts and Deleted accounts
// TODO - Get Foreign Working
// TODO - Debts
// TODO - What happens if give a child account to already existing account with no children

// TODO - Flag to toggle listing all accounts in new tra by default
// TODO - Flag to toggle asking currency

/* case 31: f.SelectAccount(); return 1; */  // TODO
// TODO - Prompt change
// TODO - List tras
// TODO - List Unreconciled tras
// TODO - Select tra
// TODO - Reconcile
// TODO - Rename except Heads and Deletes
// TODO - Give Different Parent Account except Heads and Deletes
// TODO - Delete except Heads and Deletes
// TODO - Return to home


/* case 32: f.SelectTra(f.transactions); return 1; */  // TODO
// TODO - Change Tag
// TODO - Change Location
// TODO - Change Earmark
// TODO - Change ToFrom
// TODO - Change Date
// TODO - Change Info
// TODO - Change Amount
// TODO - Reconcile
// TODO - Unreconcile
// TODO - Delete
// TODO - Return to home


// TODO - Select Macro
// TODO - Prompt change
// TODO - Add Macro
// TODO - Edit Macro
// TODO - Run Macro
// TODO - print Macro
// TODO - Return to home

/* case 61: f.NewYear(); return 1; */  // TODO

class Account
{
	public:
		string Name;
		double Amount;
		string Type;
		string Currency;

		map<string, Account*> Children;
		Account* Parent;

		Account();
		Account(string n, double a, string t, string c);

		json AsJson();
		void FromJson(const json& j);

		void FromJsonError(const string& s);

		void Print(const string& indent);
};

class Tra
{
	public:
		string Type;     // "Transaction" or "Transfer"

		unsigned long long Id;
		unsigned long long Date;
		string Info;
		int Reconciled;
		string Currency;
		double Amount;
		double DefaultCurrencyAmount;

		// Transaction
		Account* Tag;
		Account* Location;
		Account* Earmark;
		Account* ToFrom;

		// Transfer
		Account* From;
		Account* To;

		json AsJson();
		void FromJson(const json& j, map<string, Account*>& AllAccounts);

		void FromJsonError(const string& s);

		Tra();
		void Print(const string& DefaultCurrency);

		void Reconcile();
};

class Macro
{
	public:
		string Name;
		map<unsigned long long, class Tra*> Tras;

		json AsJson();
		void FromJson(const json& j, map<string, Account*>& AllAccounts);

		void FromJsonError(const string& s);

		Macro();
};

class CurrencyConversion
{
	public:
		string ForeignCurrency;
		double AmountForeign;
		double AmountDefault;

		CurrencyConversion();

		json AsJson();
		void FromJson(const json& j);

		void FromJsonError(const string& s);
};


class Finances
{
	protected:
		void LoadFromFile();
		void SaveToFile(int StartNewTimePeriodFromHere);
		void FromJson(const json& j);
		json AsJson();

		void FromJsonError(const string& s);

		void Clear();

		int InteractWithUser();
		void GetCommand(vector<string>& CommandVec, const string& Prompt);

		void PrintSomething(const vector<string>& CommandVec);
		void PrintTras(int num, int OnlyUnreconciled);
		void PrintAccounts(const string& which);

		void NewSomething(const vector<string>& CommandVec);
		void NewAccount(const string& acc_n);
		void NewTra(int PrintAccountsByDefault);

		int GetNewTransactionAccounts(string& tag_n,string& loc_n,string& ear_n,string& tf_n, int PrintAccountsByDefault);
		int GetNewTransactionAccountsInner(string& acc_n, const string& type, int PrintAccountsByDefault);
		int GetNewTransferAccounts(string& from_n,string& to_n,int PrintAccountsByDefault);
		int GetNewTransferAccountsInner(string& acc_n, const string& type, int PrintAccountsByDefault);
		void RecordTra(Tra* tra);
		void PercolateTra(Tra* tra, Account* a);

		void Reconcile(Account* acc);

		int AskWhetherToSave();

		string filename;
		string DefaultCurrency;

		map<string, Account*> AllAccounts;

		Account* HeadTag;
		Account* HeadLocation;
		Account* HeadEarmark;
		Account* HeadToFrom;

		map<string, Macro*> Macros;
		map<unsigned long long, Tra*> Tras;
		map<string, CurrencyConversion*> CurrencyConversions;


	public:
		Finances(const string& fn);
		void Run();
};

#endif
