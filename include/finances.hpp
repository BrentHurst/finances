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
const string Prompt = "~~~~>";

// TODO - Write some checks for after reading

class Account
{
	public:
		string Name;
		double Amount;
		string Type;
		string Currency;

		map<string, Account*> Children;
		Account* Parent;

		/* map<unsigned long long, class Tra*> Tras; */

		// TODO - Reconcile Functions
		// TODO - Print - example commented out

		Account();

		json AsJson();
		void FromJson(const json& j);

		void FromJsonError(const string& s);
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

		// TODO - Reconcile
		// TODO - Print - example commented out
		// TODO - Constructors - examples commented out
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
		// TODO - Modify
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
		void GetCommand(vector<string>& CommandVec);

		void PrintSomething(const vector<string>& CommandVec);
		void PrintTras();

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
