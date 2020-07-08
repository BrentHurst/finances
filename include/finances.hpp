#ifndef FINANCES_H
#define FINANCES_H

#include <map>
#include <string>
#include <vector>
#include "utilities.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using nlohmann::json;


class Account
{
	public:
		double Amount;
		string Name;
		string Type;

		map<string, Account*> SubAccounts;
		Account* SuperAccount;

		vector<class Tra*> Tras;


		Account();
		// TODO - Reconcile Functions
		// TODO - Print - example commented out

		// TODO - AsJson()
		// TODO - FromJson()

		// TODO - Add foreign currency option
		/* string Currency; */
		/* int ForeignCurrency; */
		/* double ForeignAmount; */
};

class Tra
{
	string Type;     // "transaction" or "transfer"

	unsigned int Id;
	/* DateClass* Date; */
	string Info;
	int Reconciled;
	double Amount;

	// Transaction
	Account* Tag;
	Account* Location;
	Account* Earmark;
	Account* ToFrom;

	// Transfer
	Account* From;
	Account* To;

	// TODO - Reconcile
	// TODO - Print - example commented out
	// TODO - Constructors - examples commented out
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
		json ToJson();


		map<int,string,cmdcomp> CmdList;
		string filename;

		string PrimaryCurrency;

	public:
		Finances(const string& fn);
		void Run();
};

#endif
