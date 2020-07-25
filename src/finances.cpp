#include "finances.hpp"
#include "nlohmann/json.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using nlohmann::json;

typedef std::runtime_error SRE;


/* int Finances::RunCommand(int cmd) */
/* { */

// TODO - all of these
/* case  1: f.ReadNewTransaction(1,0); return 1; */
/* case  2: f.ReadNewTransfer(1,0); return 1; */
/* case  3: f.ReadNewForeignTransaction(); return 1; */
/* case  4: f.ReadTransferToForeign(); return 1; */
/* case  5: f.ReadTransferFromForeign(); return 1; */
/* case  6: f.ReadNewAccount(); return 1; */

/* case 11: f.PrintTransactions(); return 1; */
/* case 12: f.PrintTransfers(); return 1; */
/* case 13: f.PrintUnreconciledTransactions(); return 1; */
/* case 14: f.PrintUnreconciledTransfers(); return 1; */

/* case 21: f.PrintAllAccounts(); return 1; */
/* case 22: f.PrintEarmarks(); return 1; */
/* case 23: f.PrintLocations(); return 1; */
/* case 24: f.PrintTags(); return 1; */
/* case 25: f.PrintTofroms(); return 1; */

/* case 31: f.SelectAccount(); return 1; */
/* case 32: f.SelectTransaction(f.transactions); return 1; */
/* case 33: f.SelectTransfer(f.transfers); return 1; */

/* case 41: f.Macros(); return 1; */

/* case 51: f.Reconcile(); return 1; */

/* case 61: f.NewYear(); return 1; */

/* } */

void Finances::Run()
{
	LoadFromFile();

	if(InteractWithUser() == 1)
		SaveToFile();
}


int Finances::InteractWithUser()
{
	vector<string> CommandVec;

	while(true)
	{
		GetCommand(CommandVec);

		if(!CommandVec.size())
		{
			// Do Nothing
		}
		else if(CommandVec[0] == "quit" || CommandVec[0] == "q")
		{
			return AskWhetherToSave();
		}
		else if(CommandVec[0] == "print" || CommandVec[0] == "p")
		{
			PrintSomething(CommandVec);
		}




	}
}

void Finances::GetCommand(vector<string>& CommandVec)
{
	string line;
	istringstream ss;
	string s;

	printf("%s ",Prompt.c_str());

	getline(cin, line);

	CommandVec.clear();
	ss.clear();
	ss.str(line);
	while(ss >> s)
		CommandVec.push_back(s);
}

int Finances::AskWhetherToSave()
{
	char c;
	do{
		printf("Do you want to save before you quit? [y/n]: ");
		c = ReadChar();
	}while(c != 'y' && c != 'n');
	return (c == 'y');
}


void Tra::Print(const string& DefaultCurrency)
{
	if(Type == "Transaction")
	{
		printf("\t%llu: %c\t%15s.\t%15s.\t%15s.\t%15s.\t%c%s%10.2lf",Id,(Reconciled ? 'R' : '-'),Tag->Name.c_str(),Location->Name.c_str(),Earmark->Name.c_str(),ToFrom->Name.c_str(),((Amount < 0) ? '-' : (Amount > 0) ? '+' : ' '),Currency.c_str(),abs_(Amount));
		if(Currency != DefaultCurrency)
			printf("  =  %c%s%10.2lf",((Amount < 0) ? '-' : (Amount > 0) ? '+' : ' '),DefaultCurrency.c_str(),abs_(DefaultCurrencyAmount));
		printf("\n\t%s\n",Info.c_str());
	}
	else
	{
		printf("\t%llu: %c\t%15s  ->  %15s\t%c%s%10.2lf",Id,(Reconciled ? 'R' : '-'),From->Name.c_str(),To->Name.c_str(),((Amount < 0) ? '-' : (Amount > 0) ? '+' : ' '),Currency.c_str(),abs_(Amount));
		if(Currency != DefaultCurrency)
			printf("  =  %c%s%10.2lf",((Amount < 0) ? '-' : (Amount > 0) ? '+' : ' '),DefaultCurrency.c_str(),abs_(DefaultCurrencyAmount));
		printf("\n\t%s\n",Info.c_str());
	}
}


void Finances::PrintSomething(const vector<string>& CommandVec)
{
	if(CommandVec.size() == 2)
	{
		if(CommandVec[1] == "tras" || CommandVec[1] == "t")
			PrintTras();
	}
}

void Finances::PrintTras()
{
	for(map<unsigned long long, Tra*>::iterator mit = Tras.begin(); mit != Tras.end(); ++mit)
		mit->second->Print(DefaultCurrency);
}
