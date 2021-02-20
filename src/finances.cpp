#include "finances.hpp"
#include "nlohmann/json.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using nlohmann::json;

typedef std::runtime_error SRE;


void Finances::Run(int newfin)
{
	if(newfin)
		NewFinances();

	LoadFromFile();

	if(InteractWithUser() == 1)
		SaveToFile(0);

	if(cin.eof())
		printf("\n");
}

void Finances::NewFinances()
{
	DefaultCurrency = ReadInDefaultCurrency();

	HeadTag = new Account(HEAD_TAG_NAME,0,"Tag",DefaultCurrency);
	HeadLocation = new Account(HEAD_LOCATION_NAME,0,"Location",DefaultCurrency);
	HeadEarmark = new Account(HEAD_EARMARK_NAME,0,"Earmark",DefaultCurrency);
	HeadToFrom = new Account(HEAD_TOFROM_NAME,0,"ToFrom",DefaultCurrency);
	AllAccounts[HeadTag->Name] = HeadTag;
	AllAccounts[HeadLocation->Name] = HeadLocation;
	AllAccounts[HeadEarmark->Name] = HeadEarmark;
	AllAccounts[HeadToFrom->Name] = HeadToFrom;

	DeleteTag = new Account(DELETE_TAG_NAME,0,"Tag",DefaultCurrency);
	DeleteLocation = new Account(DELETE_LOCATION_NAME,0,"Location",DefaultCurrency);
	DeleteEarmark = new Account(DELETE_EARMARK_NAME,0,"Earmark",DefaultCurrency);
	DeleteToFrom = new Account(DELETE_TOFROM_NAME,0,"ToFrom",DefaultCurrency);
	AllAccounts[DeleteTag->Name] = DeleteTag;
	AllAccounts[DeleteLocation->Name] = DeleteLocation;
	AllAccounts[DeleteEarmark->Name] = DeleteEarmark;
	AllAccounts[DeleteToFrom->Name] = DeleteToFrom;
	DeleteTag->Parent = HeadTag;
	DeleteLocation->Parent = HeadLocation;
	DeleteEarmark->Parent = HeadEarmark;
	DeleteToFrom->Parent = HeadToFrom;
	HeadTag->Children[DeleteTag->Name] = DeleteTag;
	HeadLocation->Children[DeleteLocation->Name] = DeleteLocation;
	HeadEarmark->Children[DeleteEarmark->Name] = DeleteEarmark;
	HeadToFrom->Children[DeleteToFrom->Name] = DeleteToFrom;

	SaveToFile(0);

	printf("Your file has been created.\n");
	printf("Type ? to see menu options.\n");

	delete HeadTag;
	delete HeadLocation;
	delete HeadEarmark;
	delete HeadToFrom;
	delete DeleteTag;
	delete DeleteLocation;
	delete DeleteEarmark;
	delete DeleteToFrom;

	AllAccounts.clear();
}

int Finances::InteractWithUser()
{
	vector<string> CommandVec;

	while(true)
	{
		GetCommand(CommandVec,DefaultPrompt);

		if(!CommandVec.size())
		{
			if(cin.eof())
				return 0;
			// else do nothing and continue
		}
		else if(CommandVec[0] == "help" || CommandVec[0] == "h" || CommandVec[0] == "?")
		{
			printf("Commands:\n");
			printf("\n");

			printf("\tquit | q -- exit program\n");
			printf("\n");

			printf("\tprint | p | l -- print information to the screen\n");
			printf("\t\tusage: p tras|utras [num]     (OR p t|ut [num])\n");
			printf("\t\tusage: p accounts|tags|locations|earmarks|tofroms|flags|macros     (OR p a|ta|l|e|tf|f|m)\n");
			printf("\n");

			printf("\tnew | n -- create either a new account, a new tra (transaction/transfer), or a new macro\n");
			printf("\t\tusage: n account|tra|macro     (OR n a|t|m)\n");
			printf("\n");

			printf("\treconcile | r -- reconcile unreconciled transactions\n");
			printf("\n");

			printf("\tselect | s -- select something to enter a new menu\n");
			printf("\t\tusage: s account|tra|flag|macro     (OR s a|t|f|m)\n");
			printf("\n");

			printf("\tny -- start new year from here\n");
			printf("\n");

			printf("\thelp | h | ? -- print help menu\n");
			printf("\n");
		}
		else if(CommandVec[0] == "quit" || CommandVec[0] == "q")
		{
			return AskWhetherToSave();
		}
		else if(CommandVec[0] == "print" || CommandVec[0] == "p" || CommandVec[0] == "l")
		{
			PrintSomething(CommandVec);
		}
		else if(CommandVec[0] == "new" || CommandVec[0] == "n")
		{
			NewSomething(CommandVec);
		}
		else if(CommandVec[0] == "reconcile" || CommandVec[0] == "r")
		{
			Reconcile(NULL);
		}
		else if(CommandVec[0] == "select" || CommandVec[0] == "s")
		{
			SelectSomething(CommandVec);
		}
		else if(CommandVec[0] == "ny")
		{
			SaveToFile(1);
		}
		else
		{
			printf("Command not recognized.\n");
		}
	}
}

void Finances::GetCommand(vector<string>& CommandVec, const string& Prompt)
{
	string line;
	istringstream ss;
	string s;

	ColorOutput("Bold Magenta");
	printf("%s ",Prompt.c_str());
	ColorOutput("Reset");

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

void Tra::Reconcile()
{
	Reconciled = 1;
}
void Tra::Unreconcile()
{
	Reconciled = 0;
}

void Tra::Print(const string& DefaultCurrency)
{
	printf("\t%llu: %c\t",(Id ? : Date),(Reconciled ? 'R' : '-'));

	if(Type == "Transaction")
		printf("%15s.\t%15s.\t%15s.\t%15s.",Tag->Name.c_str(),Location->Name.c_str(),Earmark->Name.c_str(),ToFrom->Name.c_str());
	else if(Type == "Transfer")
		printf("%15s  ->  %15s",From->Name.c_str(),To->Name.c_str());

	printf("\t");
	PrintCurrencyAmount(Currency,Amount);
	if(Currency != DefaultCurrency)
	{
		printf("  =  ");
		PrintCurrencyAmount(DefaultCurrency,DefaultCurrencyAmount);
	}
	printf("\n");
	if(Info.size())
		printf("\t\t%s\n",Info.c_str());
}


void Account::Print(const string& indent)
{
	printf("%s",indent.c_str());
	PrintCurrencyAmount(Currency,Amount);

	if(Amount < 0)
		ColorOutput("Bold Red");
	else if(Amount > 0)
		ColorOutput("Bold Cyan");
	else if(Amount == 0)
		ColorOutput("Cyan");

	printf("  %s\n",Name.c_str());

	ColorOutput("Reset");

	for(map<string,Account*>::iterator mit = Children.begin(); mit != Children.end(); ++mit)
		mit->second->Print(indent + "\t");
}


void Finances::PrintSomething(const vector<string>& CommandVec)
{
	if(CommandVec.size() >= 2)
	{
		if(CommandVec[1] == "tras" || CommandVec[1] == "t")
		{
			if(CommandVec.size() == 2)
				PrintTras(0,0,NULL);
			else
				PrintTras(stoi_(CommandVec[2]),0,NULL);
		}
		else if(CommandVec[1] == "utras" || CommandVec[1] == "ut" || CommandVec[1] == "tu")
		{
			if(CommandVec.size() == 2)
				PrintTras(0,1,NULL);
			else
				PrintTras(stoi_(CommandVec[2]),1,NULL);
		}
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
		else if(CommandVec[1] == "flags" || CommandVec[1] == "f")
			PrintFlags();
		else if(CommandVec[1] == "macros" || CommandVec[1] == "m")
			PrintMacros();
	}
}

void Finances::PrintFlags()
{
	for(map<string,int>::iterator mit = Flags.begin(); mit != Flags.end(); ++mit)
		printf("\t%d - %s\n",mit->second,mit->first.c_str());
}

void Finances::PrintMacros()
{
	for(map<string,Macro*>::iterator mit = Macros.begin(); mit != Macros.end(); ++mit)
	{
		mit->second->Print();
		printf("\n\n\n");
	}
}

void Finances::PrintTras(int num, int OnlyUnreconciled, Account* acc)
{
	map<unsigned long long, Tra*>::iterator mit;
	int i;
	unsigned long long lastDate;

	if(Tras.size() == 0)
	{
		printf("There are no tras to print.\n");
		return;
	}

	if(num > 0)
	{
		mit = Tras.end();
		for(--mit , i = 0; mit != Tras.begin() && i < num; --mit)
			if(!acc || IsAccountPartOfTra(acc,mit->second))
				i++;
	}
	else
		mit = Tras.begin();

	lastDate = mit->second->Date;
	for( ; mit != Tras.end(); ++mit)
		if(!OnlyUnreconciled || !mit->second->Reconciled)
		{
			if(!acc || IsAccountPartOfTra(acc,mit->second))
			{
				if(mit->second->Date != lastDate)
					printf("\n\n");
				mit->second->Print(DefaultCurrency);
				lastDate = mit->second->Date;
			}
		}
}

void Finances::PrintAccounts(const string& which)
{
	if(which == "a" || which == "t" || which == "All" || which == "Tag" || which == "Tags")
	{
		HeadTag->Print("\t");
		printf("\n\n\n");
	}
	if(which == "a" || which == "l" || which == "All" || which == "Location" || which == "Locations")
	{
		HeadLocation->Print("\t");
		printf("\n\n\n");
	}
	if(which == "a" || which == "e" || which == "All" || which == "Earmark" || which == "Earmarks")
	{
		HeadEarmark->Print("\t");
		printf("\n\n\n");
	}
	if(which == "a" || which == "tf" || which == "All" || which == "ToFrom" || which == "ToFroms")
	{
		HeadToFrom->Print("\t");
		printf("\n\n\n");
	}
}


void Finances::NewSomething(const vector<string>& CommandVec)
{
	string s;

	if(CommandVec.size() >= 2)
	{
		if(CommandVec[1] == "account" || CommandVec[1] == "acc" || CommandVec[1] == "a")
		{
			NewAccount("");
		}
		else if(CommandVec[1] == "tra" || CommandVec[1] == "t")
		{
			if(CommandVec.size() == 2)
				NewTra(Flags["ListAccountsForNewTrasByDefault"]);
			else if(CommandVec[2] == "-pa")
				NewTra(1);
			else if(CommandVec[2] == "-dpa")
				NewTra(0);
			else
				printf("Unrecognized option: %s\n",CommandVec[2].c_str());
		}
		else if(CommandVec[1] == "macro" || CommandVec[1] == "m")
		{
			s = ReadInMacroName();
			if(Macros.find(s) == Macros.end())
			{
				Macros[s] = new Macro;
				printf("Macro \"%s\" successfully created.\n",s.c_str());
			}
			else
			{
				printf("Macro with name \"%s\" already exists.\n",s.c_str());
			}
		}
	}
}

void Finances::NewAccount(const string& acc_n)
{
	string name;
	string cur;
	string par;
	Account* acc;
	Account* paracc;

	if(acc_n.size())
		name = acc_n;
	else
	{
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
	}

	if(Flags["AskCurrency"])
		cur = ReadInCurrency();
	else
		cur = DefaultCurrency;

	par = ReadInParentAccountName();
	while(AllAccounts.find(par) == AllAccounts.end())
	{
		if(AskAddNonexistentAccount(par))
		{
			NewAccount(par);
			continue;
		}

		if(!AskTryAgain("There is no account with the name \"" + par + "\"."))
		{
			printf("Discarding new account.\n");
			return;
		}

		PrintAccounts("All");

		par = ReadInParentAccountName();
	}

	if(!AskAccurateAccount(name,cur,par))
	{
		printf("Discarding new account.\n");
		return;
	}

	paracc = AllAccounts[par];
	acc = new Account(name,0,paracc->Type,cur);

	ReparentCP(acc,paracc);

	AllAccounts[acc->Name] = acc;

	printf("Account \"%s\" successfully created.\n",name.c_str());
}

void Finances::NewTra(int PrintAccountsByDefault)
{
	Tra* tra;
	string type;
	unsigned long long date;
	string cur;
	double amt;
	string info;
	string tag_n;
	string loc_n;
	string ear_n;
	string tf_n;
	string to_n;
	string from_n;

	type = ReadInType();
	date = ReadInDate();

	if(type == "Transaction")
	{
		if(!GetNewTransactionAccounts(tag_n,loc_n,ear_n,tf_n,PrintAccountsByDefault))
		{
			printf("Transaction has been discarded.\n");
			return;
		}
	}
	else if(type == "Transfer")
	{
		if(!GetNewTransferAccounts(from_n,to_n,PrintAccountsByDefault))
		{
			printf("Transfer has been discarded.\n");
			return;
		}
	}
	else
	{
		fprintf(stderr,"Error 4: Type isn't Transaction or Transfer.\n");
		return;
	}

	if(Flags["AskCurrency"])
	{
		cur = ReadInCurrency();
	}
	else
	{
		cur = DefaultCurrency;
		printf("Currency set to default currency: %s\n",DefaultCurrency.c_str());
	}

	amt = ReadInAmount();
	info = ReadInInfo();

	tra = new Tra;
	tra->Type = type;
	tra->Date = date;
	tra->Info = info;
	tra->Currency = cur;
	tra->Amount = amt;
	if(cur == DefaultCurrency)
		tra->DefaultCurrencyAmount = amt;
	else
	{
		// TODO - else if cur != DefaultCurrencyAmount
	}
	if(tra->Type == "Transaction")
	{
		tra->Tag = AllAccounts[tag_n];
		tra->Location = AllAccounts[loc_n];
		tra->Earmark = AllAccounts[ear_n];
		tra->ToFrom = AllAccounts[tf_n];
	}
	else if(tra->Type == "Transfer")
	{
		tra->To = AllAccounts[to_n];
		tra->From = AllAccounts[from_n];
	}
	else
	{
		fprintf(stderr,"Error 5: Type isn't Transaction or Transfer.\n");
		delete tra;
		return;
	}


	if(!AskAccurateTra(tra, DefaultCurrency))
	{
		printf("%s has been discarded.\n",tra->Type.c_str());
		delete tra;
		return;
	}

	RecordTra(tra);
	printf("%s successfully recorded.\n",tra->Type.c_str());
}


int Finances::GetNewTransactionAccounts(string& tag_n,string& loc_n,string& ear_n,string& tf_n,int PrintAccountsByDefault)
{
	if(!GetNewTransactionAccountsInner(tag_n, "Tag", PrintAccountsByDefault))
		return 0;

	if(!GetNewTransactionAccountsInner(loc_n, "Location", PrintAccountsByDefault))
		return 0;

	if(!GetNewTransactionAccountsInner(ear_n, "Earmark", PrintAccountsByDefault))
		return 0;

	if(!GetNewTransactionAccountsInner(tf_n, "ToFrom", PrintAccountsByDefault))
		return 0;

	return 1;
}

int Finances::GetNewTransactionAccountsInner(string& acc_n, const string& type, int PrintAccountsByDefault)
{
	if(PrintAccountsByDefault)
		PrintAccounts(type);
	acc_n = ReadInNewTraAccount(type);

	while(AllAccounts.find(acc_n) == AllAccounts.end() || AllAccounts[acc_n]->Type != type || AllAccounts[acc_n]->Children.size())
	{
		if(AllAccounts.find(acc_n) == AllAccounts.end())
		{
			if(AskAddNonexistentAccount(acc_n))
			{
				NewAccount(acc_n);
				continue;
			}
			else
			{
				if(!AskTryAgain(""))
					return 0;
			}
		}
		else if(AllAccounts[acc_n]->Type != type)
		{
			if(!AskTryAgain("Account \"" + acc_n + "\" is of type \"" + AllAccounts[acc_n]->Type + "\", not \"" + type + "\"."))
				return 0;
		}
		else
		{
			if(!AskTryAgain("Account \"" + acc_n + "\" has child accounts."))
				return 0;
		}

		PrintAccounts(type);

		acc_n = ReadInNewTraAccount(type);
	}

	return 1;
}

int Finances::GetNewTransferAccounts(string& from_n,string& to_n,int PrintAccountsByDefault)
{
	if(!GetNewTransferAccountsInner(from_n,"From", PrintAccountsByDefault))
		return 0;

	if(!GetNewTransferAccountsInner(to_n,"To", PrintAccountsByDefault))
		return 0;

	if(from_n == to_n)
	{
		printf("Trying to transfer funds from an account to itself.");
		return 0;
	}

	while(AllAccounts[from_n]->Type != AllAccounts[to_n]->Type)
	{
		if(!AskTryAgain("These two accounts aren't of the same Type."))
			return 0;

		if(!GetNewTransferAccountsInner(from_n,"From",PrintAccountsByDefault))
			return 0;

		if(!GetNewTransferAccountsInner(to_n,"To",PrintAccountsByDefault))
			return 0;

		if(from_n == to_n)
		{
			printf("Trying to transfer funds from an account to itself.");
			return 0;
		}
	}

	return 1;
}

int Finances::GetNewTransferAccountsInner(string& acc_n, const string& type, int PrintAccountsByDefault)
{
	if(PrintAccountsByDefault)
		PrintAccounts("All");
	acc_n = ReadInNewTraAccount(type);

	while(AllAccounts.find(acc_n) == AllAccounts.end() || AllAccounts[acc_n]->Children.size())
	{
		if(AllAccounts.find(acc_n) == AllAccounts.end())
		{
			if(AskAddNonexistentAccount(acc_n))
			{
				NewAccount(acc_n);
				continue;
			}
			else
			{
				if(!AskTryAgain(""))
					return 0;
			}
		}
		else
		{
			if(!AskTryAgain("Account \"" + acc_n + "\" has child accounts."))
				return 0;
		}

		PrintAccounts("All");

		acc_n = ReadInNewTraAccount(type);
	}

	return 1;
}

void Finances::RecordTra(Tra* tra)
{
	if(tra->Type == "Transaction")
	{
		PercolateTra(tra,tra->Tag);
		PercolateTra(tra,tra->Location);
		PercolateTra(tra,tra->Earmark);
		PercolateTra(tra,tra->ToFrom);
	}
	else if(tra->Type == "Transfer")
	{
		UnPercolateTra(tra,tra->From);
		PercolateTra(tra,tra->To);
	}

	InsertTraIntoMap(tra,Tras);
}

void Finances::UnPercolateTra(Tra* tra, Account* a)
{
	// foreign TODO
	tra->Amount *= -1;
	PercolateTra(tra,a);
	tra->Amount *= -1;
}

void Finances::PercolateTra(Tra* tra, Account* a)
{
	/* if(foreign) */ // TODO
	Account* tmp;

	for(tmp = a; tmp; tmp = tmp->Parent)
	{
		tmp->Amount = Round2Decimals(tmp->Amount + tra->Amount);
	}
}

void Finances::Reconcile(Account* acc)
{
	for(map<unsigned long long,Tra*>::iterator mit = Tras.begin(); mit != Tras.end(); ++mit)
		if(!mit->second->Reconciled && (!acc || IsAccountPartOfTra(acc,mit->second)))
			if(AskReconcileTra(mit->second,DefaultCurrency))
				mit->second->Reconcile();

	printf("\nYour reconciliations have been recorded.\n");
}

void Macro::Print()
{
	string s = "$";
	printf("\n%s\n",Name.c_str());
	for(map<unsigned long long, Tra*>::iterator mit = Tras.begin(); mit != Tras.end(); ++mit)
		mit->second->Print(s); //TODO - Change when doing foreign
}
