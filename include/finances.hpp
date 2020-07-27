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
// TODO - Be able to modify Macro
// TODO - Debts
// TODO - Deleted accounts except HeadAccounts and Deleted accounts
// TODO - option to print out accounts by default
// TODO - What happens if give a child account to already existing account with no children

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
		/* void PrintCommands(); */
		/* int ReadInCommand(); */
		/* int RunCommand(int cmd); */

		void LoadFromFile();
		void SaveToFile();
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
		void NewTra();

		int GetNewTransactionAccounts(string& tag_n,string& loc_n,string& ear_n,string& tf_n);
		int GetNewTransactionAccountsInner(string& acc_n, const string& type);
		int GetNewTransferAccounts(string& from_n,string& to_n);
		int GetNewTransferAccountsInner(string& acc_n, const string& type);
		void RecordTra(Tra* tra);
		void PercolateTra(Tra* tra, Account* a);

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
