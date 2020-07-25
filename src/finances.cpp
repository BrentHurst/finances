#include "finances.hpp"
#include "nlohmann/json.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using nlohmann::json;

typedef std::runtime_error SRE;


/* case  1: f.ReadNewTra(1,0); return 1; */  // TODO

/* case 31: f.SelectAccount(); return 1; */  // TODO
/* case 32: f.SelectTra(f.transactions); return 1; */  // TODO

/* case 41: f.Macros(); return 1; */  // TODO

/* case 51: f.Reconcile(); return 1; */  // TODO

/* case 61: f.NewYear(); return 1; */  // TODO


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
		else if(CommandVec[0] == "help" || CommandVec[0] == "h" || CommandVec[0] == "?")
		{
			// TODO
			printf("I should probably have this print a help menu.\n");
		}
		else if(CommandVec[0] == "quit" || CommandVec[0] == "q")
		{
			return AskWhetherToSave();
		}
		else if(CommandVec[0] == "print" || CommandVec[0] == "p")
		{
			PrintSomething(CommandVec);
		}
		else if(CommandVec[0] == "new" || CommandVec[0] == "n")
		{
			NewSomething(CommandVec);
		}
		else
		{
			printf("Command not recognized.\n");
		}
	}
}

void Finances::GetCommand(vector<string>& CommandVec)
{
	string line;
	istringstream ss;
	string s;

	printf("%s ",Prompt.c_str());

	line = ReadString();

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
		printf("\t%llu: %c\t%15s.\t%15s.\t%15s.\t%15s.\t",Id,(Reconciled ? 'R' : '-'),Tag->Name.c_str(),Location->Name.c_str(),Earmark->Name.c_str(),ToFrom->Name.c_str());
		PrintCurrencyAmount(Currency,Amount);
		if(Currency != DefaultCurrency)
		{
			printf("  =  ");
			PrintCurrencyAmount(DefaultCurrency,DefaultCurrencyAmount);
		}
		printf("\n\t%s\n",Info.c_str());
	}
	else
	{
		printf("\t%llu: %c\t%15s  ->  %15s\t",Id,(Reconciled ? 'R' : '-'),From->Name.c_str(),To->Name.c_str());
		PrintCurrencyAmount(Currency,Amount);
		if(Currency != DefaultCurrency)
		{
			printf("  =  ");
			PrintCurrencyAmount(DefaultCurrency,DefaultCurrencyAmount);
		}
		printf("\n\t%s\n",Info.c_str());
	}
}


void Account::Print(const string& indent)
{
	printf("%s",indent.c_str());
	PrintCurrencyAmount(Currency,Amount);
	printf("  %s\n",Name.c_str());

	for(map<string,Account*>::iterator mit = Children.begin(); mit != Children.end(); ++mit)
		mit->second->Print(indent + "\t");
}


void Finances::PrintSomething(const vector<string>& CommandVec)
{
	if(CommandVec.size() == 2)
	{
		if(CommandVec[1] == "tras" || CommandVec[1] == "t")
			PrintTras();
		else if(CommandVec[1] == "utras" || CommandVec[1] == "ut" || CommandVec[1] == "tu")
			PrintUnreconciledTras();
		else if(CommandVec[1] == "accounts" || CommandVec[1] == "a")
			PrintAccounts("a");
		else if(CommandVec[1] == "tags" || CommandVec[1] == "ta")
			PrintAccounts("t");
		else if(CommandVec[1] == "locations" || CommandVec[1] == "locs" || CommandVec[1] == "loc" || CommandVec[1] == "l")
			PrintAccounts("l");
		else if(CommandVec[1] == "earmarks" || CommandVec[1] == "e")
			PrintAccounts("e");
		else if(CommandVec[1] == "tofroms" || CommandVec[1] == "tf")
			PrintAccounts("tf");
	}
}

void Finances::PrintTras()
{
	for(map<unsigned long long, Tra*>::iterator mit = Tras.begin(); mit != Tras.end(); ++mit)
		mit->second->Print(DefaultCurrency);
}
void Finances::PrintUnreconciledTras()
{
	for(map<unsigned long long, Tra*>::iterator mit = Tras.begin(); mit != Tras.end(); ++mit)
		if(!mit->second->Reconciled)
			mit->second->Print(DefaultCurrency);
}

void Finances::PrintAccounts(const string& which)
{
	if(which == "a" || which == "t")
	{
		HeadTag->Print("\t");
		printf("\n\n\n\n");
	}
	if(which == "a" || which == "l")
	{
		HeadLocation->Print("\t");
		printf("\n\n\n\n");
	}
	if(which == "a" || which == "e")
	{
		HeadEarmark->Print("\t");
		printf("\n\n\n\n");
	}
	if(which == "a" || which == "tf")
	{
		HeadToFrom->Print("\t");
		printf("\n\n\n\n");
	}
}


void Finances::NewSomething(const vector<string>& CommandVec)
{
	if(CommandVec.size() == 2)
	{
		if(CommandVec[1] == "account" || CommandVec[1] == "acc" || CommandVec[1] == "a")
		{
			NewAccount();
		}
		else if(CommandVec[1] == "tra" || CommandVec[1] == "t")
		{
			NewTra();
		}
	}
}

void Finances::NewAccount()
{
	string name;
	string cur;
	string par;
	Account* acc;
	Account* paracc;

	name = ReadInNewAccountName();
	while(AllAccounts.find(name) != AllAccounts.end())
	{
		if(!AskTryAgain("An account with the name \"" + name + "\" already exists."))
		{
			printf("Discarding new account.\n");
			return;
		}
		name = ReadInNewAccountName();
	}

	cur = ReadInCurrency();

	par = ReadInParentAccountName();
	while(AllAccounts.find(par) == AllAccounts.end())
	{
		if(!AskTryAgain("There is no account with the name \"" + par + "\"."))
		{
			printf("Discarding new account.\n");
			return;
		}
		par = ReadInParentAccountName();
	}

	if(!AskAccurateAccount(name,cur,par))
	{
		printf("Discarding new account.\n");
		return;
	}

	paracc = AllAccounts[par];
	acc = new Account(name,0,paracc->Type,cur);
	paracc->Children[acc->Name] = acc;
	acc->Parent = paracc;
	AllAccounts[acc->Name] = acc;

	printf("Account \"%s\" successfully created.\n",name.c_str());
}

void Finances::NewTra()
{
	// TODO
}
