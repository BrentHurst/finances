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

class Account
{
	public:
		string Name;
		double Amount;
		string Type;
		string Currency;

		map<string, Account*> Children;
		Account* Parent;

		vector<class Tra*> Tras;

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
		string Type;     // "transaction" or "transfer"

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

		// TODO - Reconcile
		// TODO - Print - example commented out
		// TODO - Constructors - examples commented out
};

class Macro
{
	public:
		string Name;
		vector<class Tra*> Tras;

		json AsJson(); // TODO
		void FromJson(const json& j, map<string, Account*>& AllAccounts); // TODO
		// TODO - Modify
};


class Finances
{
	protected:
		void FillCmdList();

		void PrintCommands();
		int ReadInCommand();
		int RunCommand(int cmd);

		void LoadFromFile();
		void SaveToFile();
		void FromJson(const json& j);
		json AsJson();

		void Clear();

		map<int,string,cmdcomp> CmdList;
		string filename;

		string DefaultCurrency;

		map<string, Account*> Tags;
		map<string, Account*> Locations;
		map<string, Account*> Earmarks;
		map<string, Account*> ToFroms;
		map<string, Account*> AllAccounts;

		Account* HeadTag;
		Account* HeadLocation;
		Account* HeadEarmark;
		Account* HeadToFrom;

		map<string, Macro*> Macros;

		vector<Tra*> Tras;

	public:
		Finances(const string& fn);
		void Run();
};

#endif
