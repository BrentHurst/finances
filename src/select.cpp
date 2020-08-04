#include "finances.hpp"
#include "utilities.hpp"
#include "nlohmann/json.hpp"
#include <string>
#include <cstdio>
#include <iostream>


using namespace std;
using nlohmann::json;

typedef std::runtime_error SRE;


void Finances::SelectSomething(const vector<string>& CommandVec)
{
	if(CommandVec.size() >= 2)
	{
		if(CommandVec[1] == "tra" || CommandVec[1] == "t")
			SelectTra();
		else if(CommandVec[1] == "account" || CommandVec[1] == "acc" || CommandVec[1] == "a")
			SelectAccount();
		else
			printf("Unrecognized option \"%s\"\n",CommandVec[1].c_str());
	}
}

void Finances::SelectTra()
{
	map<unsigned long long,Tra*>::iterator mit;
	unsigned long long traid;
	Tra* tra;

	traid = ReadInTraId();

	if((mit = Tras.find(traid)) == Tras.end())
	{
		printf("Transaction/Transfer with Id %llu doesn't exist.\n",traid);
		return;
	}

	tra = mit->second;
	tra->Print(DefaultCurrency);

	InteractWithUserTra(tra);
}

void Finances::InteractWithUserTra(Tra* tra)
{
	char c;
	string prompt;
	vector<string> CommandVecTra;

	prompt = DefaultPrompt;
	c = prompt.back();
	prompt.pop_back();
	prompt += ulltos_(tra->Id);
	prompt.push_back(c);

	while(true)
	{
		GetCommand(CommandVecTra,prompt);

		if(!CommandVecTra.size())
		{
			if(cin.eof())
				return;
			// else do nothing and continue
		}
		else if(CommandVecTra[0] == "help" || CommandVecTra[0] == "h" || CommandVecTra[0] == "?")
		{
			// TODO
			printf("I should probably have this print a help menu.\n");
		}
		else if(CommandVecTra[0] == "back" || CommandVecTra[0] == "b")
		{
			return;
		}
		else if(CommandVecTra[0] == "reconcile" || CommandVecTra[0] == "r")
		{
			tra->Reconcile();
			printf("Your %s has been reconciled.\n",tra->Type.c_str());
		}
		else if(CommandVecTra[0] == "unreconcile" || CommandVecTra[0] == "ur")
		{
			tra->Unreconcile();
			printf("Your %s has been unreconciled.\n",tra->Type.c_str());
		}
		else if(CommandVecTra[0] == "delete")
		{
			DeleteTra(tra);
			return;
		}
		else if(CommandVecTra[0] == "print" || CommandVecTra[0] == "p" || CommandVecTra[0] == "l")
		{
			tra->Print(DefaultCurrency);
		}
		else if(CommandVecTra[0] == "change" || CommandVecTra[0] == "c")
		{
			ChangeSomething(tra);
		}
	}
}

void Finances::DeleteTra(Tra* tra)
{
	// TODO - something with foreign probably

	if(!tra)
		return;

	if(!AskConfirmDeleteTra(tra,DefaultCurrency))
		return;

	if(tra->Type == "Transaction")
	{
		UnPercolateTra(tra,tra->Tag);
		UnPercolateTra(tra,tra->Location);
		UnPercolateTra(tra,tra->Earmark);
		UnPercolateTra(tra,tra->ToFrom);
	}
	else
	{
		PercolateTra(tra,tra->From);
		UnPercolateTra(tra,tra->To);
	}

	Tras.erase(tra->Id);

	printf("%s %llu has been deleted.\n",tra->Type.c_str(),tra->Id);

	delete tra;
}

void Finances::ChangeSomething(Tra* tra)
{
	// TODO - Change currency
	// TODO

	string AccountTypeToChange;

	tra->Print(DefaultCurrency);

	AccountTypeToChange = ReadInAccountTypeToChange();

	if(AccountTypeToChange == "q")
		return;

	if(tra->Type == "Transaction")
	{
		if(AccountTypeToChange == "From" || AccountTypeToChange == "To")
		{
			printf("Attribute \"%s\" can only be changed on Transfers.\n",AccountTypeToChange.c_str());
			return;
		}
	}
	else
	{
		if(AccountTypeToChange == "Tag" || AccountTypeToChange == "Location" || AccountTypeToChange == "Earmark" || AccountTypeToChange == "ToFrom")
		{
			printf("Attribute \"%s\" can only be changed on Transactions.\n",AccountTypeToChange.c_str());
			return;
		}
	}

	if(AccountTypeToChange == "Tag" || AccountTypeToChange == "Location" || AccountTypeToChange == "Earmark" | AccountTypeToChange == "ToFrom")
		ChangeTransactionAccount(tra,AccountTypeToChange);
	else if(AccountTypeToChange == "From" || AccountTypeToChange == "To")
		ChangeTransferAccount(tra,AccountTypeToChange);
	else if(AccountTypeToChange == "Date")
		ChangeTraDate(tra);
	else if(AccountTypeToChange == "Info")
		ChangeTraInfo(tra);
	else if(AccountTypeToChange == "Amount")
		ChangeTraAmount(tra);
	else
	{
		printf("Brent apparently forgot to code something because this line of code should never run....\n");
		printf("Please contact him and let him know. Tell him \"Error 8\". Thanks.\n");
	}
}

void Finances::ChangeTransactionAccount(Tra* tra, const string& AccountTypeToChange)
{
	// TODO - tra - Change Tag
	// TODO - tra - Change Location
	// TODO - tra - Change Earmark
	// TODO - tra - Change ToFrom
}
void Finances::ChangeTransferAccount(Tra* tra, const string& AccountTypeToChange)
{
	// TODO - tra - Change From
	// TODO - tra - Change To
}
void Finances::ChangeTraDate(Tra* tra)
{
	// TODO - tra - Change Date
}
void Finances::ChangeTraInfo(Tra* tra)
{
	string oldinfo = tra->Info;
	tra->Info = ReadInInfo();
	if(!AskAccurateChangedTra(tra,DefaultCurrency))
	{
		tra->Info = oldinfo;
		printf("Your changes have been undone.\n");
	}
	else
	{
		printf("Your changes have been saved.\n");
	}
}
void Finances::ChangeTraAmount(Tra* tra)
{
	// TODO - tra - Change Amount
}

void Finances::SelectAccount()
{
	// TODO
}

