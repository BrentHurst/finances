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

	if(AccountTypeToChange == "Tag" || AccountTypeToChange == "Location" || AccountTypeToChange == "Earmark" || AccountTypeToChange == "ToFrom")
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
	string NewAcctName;
	Account* oldacc;
	Account* newacc;

	if(!GetNewTransactionAccountsInner(NewAcctName, AccountTypeToChange, 0))
	{
		printf("Your account has been left as it was.\n");
		return;
	}

	newacc = AllAccounts[NewAcctName];

	if(AccountTypeToChange == "Tag")
	{
		oldacc = tra->Tag;
		tra->Tag = newacc;
	}
	else if(AccountTypeToChange == "Location")
	{
		oldacc = tra->Location;
		tra->Location = newacc;
	}
	else if(AccountTypeToChange == "Earmark")
	{
		oldacc = tra->Earmark;
		tra->Earmark = newacc;
	}
	else if(AccountTypeToChange == "ToFrom")
	{
		oldacc = tra->ToFrom;
		tra->ToFrom = newacc;
	}


	if(!AskAccurateChangedTra(tra,DefaultCurrency))
	{
		if(AccountTypeToChange == "Tag")
			tra->Tag = oldacc;
		else if(AccountTypeToChange == "Location")
			tra->Location = oldacc;
		else if(AccountTypeToChange == "Earmark")
			tra->Earmark = oldacc;
		else if(AccountTypeToChange == "ToFrom")
			tra->ToFrom = oldacc;

		printf("Your changes have been undone.\n");
	}
	else
	{
		UnPercolateTra(tra, oldacc);
		PercolateTra(tra,newacc);

		tra->Unreconcile();

		printf("Your changes have been saved.\n");
	}
}
void Finances::ChangeTransferAccount(Tra* tra, const string& AccountTypeToChange)
{
	string NewAcctName;
	Account* oldacc;
	Account* newacc;

	if(!GetNewTransferAccountsInner(NewAcctName, AccountTypeToChange, 0))
	{
		printf("Your account has been left as it was.\n");
		return;
	}

	newacc = AllAccounts[NewAcctName];

	if(newacc->Type != tra->From->Type)
	{
		printf("%s is of type %s, but this Transfer involves accounts of type %s.\n",NewAcctName.c_str(),newacc->Type.c_str(),tra->From->Type.c_str());
		printf("Your changes have been discarded and your transfer is being left as it was.\n");
		return;
	}

	if(AccountTypeToChange == "From")
	{
		oldacc = tra->From;
		tra->From = newacc;
	}
	else if(AccountTypeToChange == "To")
	{
		oldacc = tra->To;
		tra->To = newacc;
	}

	if(!AskAccurateChangedTra(tra,DefaultCurrency))
	{
		if(AccountTypeToChange == "From")
			tra->From = oldacc;
		else if(AccountTypeToChange == "To")
			tra->To = oldacc;

		printf("Your changes have been undone.\n");
	}
	else
	{
		if(AccountTypeToChange == "From")
		{
			PercolateTra(tra, oldacc);
			UnPercolateTra(tra, newacc);
		}
		else if(AccountTypeToChange == "To")
		{
			UnPercolateTra(tra, oldacc);
			PercolateTra(tra, newacc);
		}

		tra->Unreconcile();

		printf("Your changes have been saved.\n");
	}
}
void Finances::ChangeTraDate(Tra* tra)
{
	unsigned long long olddate = tra->Date;
	unsigned long long oldid = tra->Id;
	tra->Date = ReadInDate();
	tra->Id = 0;
	if(!AskAccurateChangedTra(tra,DefaultCurrency))
	{
		tra->Date = olddate;
		tra->Id = oldid;
		printf("Your changes have been undone.\n");
	}
	else
	{
		Tras.erase(oldid);
		InsertTraIntoMap(tra,Tras);
		printf("Your changes have been saved.\n");
	}
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
	// TODO - foreign

	double newamt;
	double oldamt = tra->Amount;
	tra->Amount = ReadInAmount();
	if(!AskAccurateChangedTra(tra,DefaultCurrency))
	{
		tra->Amount = oldamt;
		printf("Your changes have been undone.\n");
	}
	else
	{
		newamt = tra->Amount;
		tra->Amount = oldamt;
		if(tra->Type == "Transaction")
		{
			UnPercolateTra(tra,tra->Tag);
			UnPercolateTra(tra,tra->Location);
			UnPercolateTra(tra,tra->Earmark);
			UnPercolateTra(tra,tra->ToFrom);

			tra->Amount = newamt;

			PercolateTra(tra,tra->Tag);
			PercolateTra(tra,tra->Location);
			PercolateTra(tra,tra->Earmark);
			PercolateTra(tra,tra->ToFrom);
		}
		else
		{
			PercolateTra(tra,tra->From);
			UnPercolateTra(tra,tra->To);

			tra->Amount = newamt;

			UnPercolateTra(tra,tra->From);
			PercolateTra(tra,tra->To);
		}

		printf("Your changes have been saved.\n");
	}
}

void Finances::SelectAccount()
{
	map<string,Account*>::iterator mit;
	string accname;
	Account* acc;

	accname = ReadInNewTraAccount("Account");

	if((mit = AllAccounts.find(accname)) == AllAccounts.end())
	{
		printf("\"%s\" doesn't exist as an account.\n",accname.c_str());
		return;
	}

	acc = mit->second;

	InteractWithUserAccount(acc);
}

void Finances::InteractWithUserAccount(Account* acc)
{
	char c;
	string prompt;
	vector<string> CommandVecAcc;

	prompt = DefaultPrompt;
	c = prompt.back();
	prompt.pop_back();
	prompt += acc->Name;
	prompt.push_back(c);

	while(true)
	{
		GetCommand(CommandVecAcc,prompt);

		if(!CommandVecAcc.size())
		{
			if(cin.eof())
				return;
			// else do nothing and continue
		}
		else if(CommandVecAcc[0] == "help" || CommandVecAcc[0] == "h" || CommandVecAcc[0] == "?")
		{
			// TODO
			printf("I should probably have this print a help menu.\n");
		}
		else if(CommandVecAcc[0] == "back" || CommandVecAcc[0] == "b")
		{
			return;
		}
		else if(CommandVecAcc[0] == "select" || CommandVecAcc[0] == "s")
		{
			if(CommandVecAcc.size() > 1 && (CommandVecAcc[1] == "tra" || CommandVecAcc[1] == "t"))
				SelectTra();
		}
		else if(CommandVecAcc[0] == "print" || CommandVecAcc[0] == "p" || CommandVecAcc[0] == "l")
		{
			PrintSomethingAcc(CommandVecAcc,acc);
		}
		else if(CommandVecAcc[0] == "reconcile" || CommandVecAcc[0] == "r")
		{
			Reconcile(acc);
		}
		else if(CommandVecAcc[0] == "delete" || CommandVecAcc[0] == "d")
		{
			if(Delete_Acc(acc))
				return;
		}
		else if(CommandVecAcc[0] == "rename" || CommandVecAcc[0] == "rn")
		{
			RenameAccount(acc);

			prompt = DefaultPrompt;
			c = prompt.back();
			prompt.pop_back();
			prompt += acc->Name;
			prompt.push_back(c);
		}
		else if(CommandVecAcc[0] == "reparent" || CommandVecAcc[0] == "rp")
		{
			ReparentAccount(acc);
		}
	}
}

void Finances::PrintSomethingAcc(const vector<string>& CommandVec, Account* acc)
{
	if(CommandVec.size() == 1)
	{
		acc->Print("\t");
	}
	else
	{
		if(CommandVec[1] == "tras" || CommandVec[1] == "t")
		{
			if(CommandVec.size() == 2)
				PrintTras(0,0,acc);
			else
				PrintTras(stoi_(CommandVec[2]),0,acc);
		}
		else if(CommandVec[1] == "utras" || CommandVec[1] == "ut" || CommandVec[1] == "tu")
		{
			if(CommandVec.size() == 2)
				PrintTras(0,1,acc);
			else
				PrintTras(stoi_(CommandVec[2]),1,acc);
		}
	}
}

int Finances::IsHeadAccount(Account* acc)
{
	return (acc == HeadTag || acc == HeadLocation || acc == HeadEarmark || acc == HeadToFrom);
}
int Finances::IsDeleteAccount(Account* acc)
{
	return (acc == DeleteTag || acc == DeleteLocation || acc == DeleteEarmark || acc == DeleteToFrom);
}

int Finances::Delete_Acc(Account* acc)
{
	Account* TargetDeleteAccount;
	map<unsigned long long, Tra*>::iterator mit;

	if(IsHeadAccount(acc))
	{
		printf("Head accounts can't be deleted.\n");
		return 0;
	}

	if(IsDeleteAccount(acc))
	{
		printf("Delete accounts can't be deleted.\n");
		return 0;
	}

	if(acc->Children.size())
	{
		printf("Accounts with child accounts can't be deleted.\n");
		return 0;
	}

	if(acc->Amount)
	{
		printf("Accounts with nonzero amounts can't be deleted.\n");
		return 0;
	}

	if(!AskConfirmDeleteAccount(acc))
		return 0;

	if(acc->Type == "Tag")
		TargetDeleteAccount = DeleteTag;
	else if(acc->Type == "Location")
		TargetDeleteAccount = DeleteLocation;
	else if(acc->Type == "Earmark")
		TargetDeleteAccount = DeleteEarmark;
	else if(acc->Type == "ToFrom")
		TargetDeleteAccount = DeleteToFrom;

	for(mit = Tras.begin(); mit != Tras.end(); ++mit)
	{
		if(mit->second->Tag == acc)
		{
			mit->second->Tag = TargetDeleteAccount;
			mit->second->Info += " ====Original " + acc->Type + " account: " + acc->Name + "==== ";
		}
		else if(mit->second->Location == acc)
		{
			mit->second->Location = TargetDeleteAccount;
			mit->second->Info += " ====Original " + acc->Type + " account: " + acc->Name + "==== ";
		}
		else if(mit->second->Earmark == acc)
		{
			mit->second->Earmark = TargetDeleteAccount;
			mit->second->Info += " ====Original " + acc->Type + " account: " + acc->Name + "==== ";
		}
		else if(mit->second->ToFrom == acc)
		{
			mit->second->ToFrom = TargetDeleteAccount;
			mit->second->Info += " ====Original " + acc->Type + " account: " + acc->Name + "==== ";
		}
		else if(mit->second->From == acc)
		{
			mit->second->From = TargetDeleteAccount;
			mit->second->Info += " ====Original From account: " + acc->Name + "==== ";
		}
		else if(mit->second->To == acc)
		{
			mit->second->To = TargetDeleteAccount;
			mit->second->Info += " ====Original To account: " + acc->Name + "==== ";
		}
	}

	for(mit = Tras.begin(); mit != Tras.end(); ++mit)
		if(IsAccountPartOfTra(acc,mit->second))
		{
			printf("Something went wrong: Error 21.%llu. Your account hasn't been completely deleted for some reason.\n",mit->second->Id);
			return 0;
		}

	AllAccounts.erase(acc->Name);
	acc->Parent->Children.erase(acc->Name);

	printf("Account \"%s\" has been deleted.\n",acc->Name.c_str());

	delete acc;

	return 1;
}

void Finances::RenameAccount(Account* acc)
{
	string newname;
	string oldname;

	if(IsHeadAccount(acc))
	{
		printf("Head accounts can't be renamed.\n");
		return;
	}

	if(IsDeleteAccount(acc))
	{
		printf("Delete accounts can't be renamed.\n");
		return;
	}

	oldname = acc->Name;

	newname = ReadInNewAccountName();
	while(AllAccounts.find(newname) != AllAccounts.end())
	{
		if(!AskTryAgain("An account with the name \"" + newname + "\" already exists."))
		{
			printf("Your account's name has not been changed.");
			return;
		}
		newname = ReadInNewAccountName();
	}

	acc->Name = newname;

	AllAccounts.erase(oldname);
	AllAccounts[newname] = acc;

	acc->Parent->Children.erase(oldname);
	acc->Parent->Children[newname] = acc;
}

void Finances::ReparentAccount(Account* acc)
{
	//TODO
}
