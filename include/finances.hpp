#ifndef FINANCES_H
#define FINANCES_H

#include <map>
#include <string>
#include <vector>
#include "utilities.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using nlohmann::json;



// TODO - Write some checks for after reading
// TODO - Get Foreign Working
// TODO - Debts
// TODO - What happens if give a child account to already existing account with no children
// TODO - Make sure deletes don't get children

// TODO - Flag to toggle listing all accounts in new tra by default
// TODO - Flag to toggle asking currency

// TODO - account - Rename except Heads and Deletes
// TODO - account - Give Different Parent Account except Heads and Deletes
// TODO - account - Delete except Heads and Deletes

// TODO - Select Macro
// TODO - Prompt change
// TODO - Add Macro
// TODO - Edit Macro
// TODO - Run Macro
// TODO - print Macro
// TODO - Return to home

/* case 61: f.NewYear(); return 1; */  // TODO

// TODO - New file



const string ErrorAsterisks = "*!*!*!*!*!*";
const string DefaultPrompt = "~~~~/>";

const string HEAD_TAG_NAME = "TAGS";
const string HEAD_LOCATION_NAME = "LOCATIONS";
const string HEAD_EARMARK_NAME = "EARMARKS";
const string HEAD_TOFROM_NAME = "TOFROMS";

const string DELETE_TAG_NAME = "!!~del_T~!!";
const string DELETE_LOCATION_NAME = "!!~del_L~!!";
const string DELETE_EARMARK_NAME = "!!~del_E~!!";
const string DELETE_TOFROM_NAME = "!!~del_TF~!!";

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
		void Unreconcile();
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

		void SetHeadsAndDeletes();

		void Clear();

		int InteractWithUser();
		void GetCommand(vector<string>& CommandVec, const string& Prompt);

		void PrintSomething(const vector<string>& CommandVec);
		void PrintTras(int num, int OnlyUnreconciled, Account* acc);
		void PrintAccounts(const string& which);

		void NewSomething(const vector<string>& CommandVec);
		void NewAccount(const string& acc_n);
		void NewTra(int PrintAccountsByDefault);

		void SelectSomething(const vector<string>& CommandVec);
		void SelectTra();
		void SelectAccount();

		void InteractWithUserTra(Tra* tra);
		void DeleteTra(Tra* tra);
		void ChangeSomething(Tra* tra);
		void ChangeTransactionAccount(Tra* tra, const string& AccountTypeToChange);
		void ChangeTransferAccount(Tra* tra, const string& AccountTypeToChange);
		void ChangeTraDate(Tra* tra);
		void ChangeTraInfo(Tra* tra);
		void ChangeTraAmount(Tra* tra);

		void InteractWithUserAccount(Account* acc);
		void PrintSomethingAcc(const vector<string>& CommandVec, Account* acc);

		int GetNewTransactionAccounts(string& tag_n,string& loc_n,string& ear_n,string& tf_n, int PrintAccountsByDefault);
		int GetNewTransactionAccountsInner(string& acc_n, const string& type, int PrintAccountsByDefault);
		int GetNewTransferAccounts(string& from_n,string& to_n,int PrintAccountsByDefault);
		int GetNewTransferAccountsInner(string& acc_n, const string& type, int PrintAccountsByDefault);
		void RecordTra(Tra* tra);
		void PercolateTra(Tra* tra, Account* a);
		void UnPercolateTra(Tra* tra, Account* a);

		void Reconcile(Account* acc);

		int AskWhetherToSave();

		void CheckHeadsAndDeletes();
		void CheckHeadAccountExists(const string& name);
		void CheckDeleteAccountExists(const string& name, const string& type, const string& parname);

		string filename;
		string DefaultCurrency;

		map<string, Account*> AllAccounts;

		Account* HeadTag;
		Account* HeadLocation;
		Account* HeadEarmark;
		Account* HeadToFrom;

		Account* DeleteTag;
		Account* DeleteLocation;
		Account* DeleteEarmark;
		Account* DeleteToFrom;

		map<string, Macro*> Macros;
		map<unsigned long long, Tra*> Tras;
		map<string, CurrencyConversion*> CurrencyConversions;


	public:
		Finances(const string& fn);
		void Run();
};

#endif
