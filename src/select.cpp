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
		else if(CommandVec[1] == "flag" || CommandVec[1] == "f")
			SelectFlag();
		else if(CommandVec[1] == "macro" || CommandVec[1] == "m")
			SelectMacro();
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
			printf("Commands:\n");
			printf("\n");

			printf("\tback | b -- return to main menu\n");
			printf("\n");

			printf("\treconcile | r -- reconcile this tra\n");
			printf("\n");

			printf("\tunreconcile | ur -- unreconcile this tra\n");
			printf("\n");

			printf("\tdelete -- delete this transaction\n");
			printf("\n");

			printf("\tprint | p | l -- print info\n");
			printf("\n");

			printf("\tchange | c -- change something about this tra\n");
			printf("\n");

			printf("\thelp | h | ? -- print help menu\n");
			printf("\n");
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

			prompt = DefaultPrompt;
			c = prompt.back();
			prompt.pop_back();
			prompt += ulltos_(tra->Id);
			prompt.push_back(c);
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
	else if(tra->Type == "Transfer")
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

	AccountTypeToChange = ReadInAccountTypeToChange(tra);

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
	else if(tra->Type == "Transfer")
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
}

void Finances::ChangeTransactionAccount(Tra* tra, const string& AccountTypeToChange)
{
	string NewAcctName;
	Account* oldacc;
	Account* newacc;

	if(!GetNewTransactionAccountsInner(NewAcctName, AccountTypeToChange, Flags["ListAccountsForNewTrasByDefault"]))
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

	if(!GetNewTransferAccountsInner(NewAcctName, AccountTypeToChange, Flags["ListAccountsForNewTrasByDefault"]))
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
		else if(tra->Type == "Transfer")
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
			printf("Commands:\n");
			printf("\n");

			printf("\tback | b -- return to main menu\n");
			printf("\n");

			printf("\tselect | s -- select a tra\n");
			printf("\t\tusage: s tra     (OR s t)\n");
			printf("\n");

			printf("\tprint | p | l -- print info\n");
			printf("\n");

			printf("\treconcile | r -- reconcile tras in this account\n");
			printf("\n");

			printf("\tdelete | d -- delete this account\n");
			printf("\n");

			printf("\trename | rn -- rename this account\n");
			printf("\n");

			printf("\treparent | rp -- change the parent account of this account\n");
			printf("\n");

			printf("\thelp | h | ? -- print help menu\n");
			printf("\n");
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

	for(map<string,Macro*>::iterator mit = Macros.begin(); mit != Macros.end(); ++mit)
		for(map<unsigned long long, Tra*>::iterator mit2 = mit->second->Tras.begin(); mit2 != mit->second->Tras.end(); ++mit2)
			if(IsAccountPartOfTra(acc,mit2->second))
			{
				printf("Account is used in macro \"%s\". Please change the macro before deleting the account.\n",mit->first.c_str());
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
	string newparname;

	newparname = ReadInParentAccountName_Reparenting();
	while(AllAccounts.find(newparname) == AllAccounts.end())
	{
		if(AskAddNonexistentAccount(newparname))
		{
			NewAccount(newparname);
		}
		else
		{
			if(!AskTryAgain(""))
				return;

			newparname = ReadInParentAccountName_Reparenting();
		}
	}

	ReparentCP(acc,AllAccounts[newparname]);
}

void Finances::SelectFlag()
{
	string flag;

	printf("\n");
	PrintFlags();
	printf("\n");

	flag = ReadInFlag();
	while(Flags.find(flag) == Flags.end())
	{
		if(!AskTryAgain("No flag with the name \"" + flag + "\" exists."))
			return;
		flag = ReadInFlag();
	}

	printf("What would you like to change the state of this flag to? 0 for off, 1 for on. [0/1]: ");
	Flags[flag] = ReadInt();
	printf("%s has been set to %d.\n",flag.c_str(),Flags[flag]);
}

void Finances::SelectMacro()
{
	map<string,Macro*>::iterator mit;
	string name;
	Macro* m;

	name = ReadInMacroName();
	if((mit = Macros.find(name)) == Macros.end())
	{
		printf("There is no macro named \"%s\".\n",name.c_str());
		return;
	}

	m = mit->second;

	InteractWithUserMacro(m);
}

void Finances::InteractWithUserMacro(Macro* macro)
{
	char c;
	string prompt;
	vector<string> CommandVecMacro;

	prompt = DefaultPrompt;
	c = prompt.back();
	prompt.pop_back();
	prompt += "Macros/";
	prompt += macro->Name;
	prompt.push_back(c);

	while(true)
	{
		GetCommand(CommandVecMacro,prompt);

		if(!CommandVecMacro.size())
		{
			if(cin.eof())
				return;
			// else do nothing and continue
		}
		else if(CommandVecMacro[0] == "help" || CommandVecMacro[0] == "h" || CommandVecMacro[0] == "?")
		{
			printf("Commands:\n");
			printf("\n");

			printf("\tback | b -- return to main menu\n");
			printf("\n");

			printf("\tprint | p | l -- print info\n");
			printf("\n");

			printf("\trename | rn -- rename this macro\n");
			printf("\n");

			printf("\tn t -- add a new tra to this macro\n");
			printf("\n");

			printf("\ts t -- select one of this macro's tras\n");
			printf("\n");

			printf("\trun -- run this macro\n");
			printf("\n");

			printf("\thelp | h | ? -- print help menu\n");
			printf("\n");
		}
		else if(CommandVecMacro[0] == "back" || CommandVecMacro[0] == "b")
		{
			return;
		}
		else if(CommandVecMacro[0] == "print" || CommandVecMacro[0] == "p" || CommandVecMacro[0] == "l")
		{
			macro->Print();
		}
		else if(CommandVecMacro[0] == "rename" || CommandVecMacro[0] == "rn")
		{
			RenameMacro(macro);

			prompt = DefaultPrompt;
			c = prompt.back();
			prompt.pop_back();
			prompt += "Macros/";
			prompt += macro->Name;
			prompt.push_back(c);
		}
		else if(CommandVecMacro[0] == "n" && CommandVecMacro.size() > 1 && CommandVecMacro[1] == "t")
		{
			macro->NewTra();
		}
		else if(CommandVecMacro[0] == "s" && CommandVecMacro.size() > 1 && CommandVecMacro[1] == "t")
		{
			macro->SelectTra();
		}
		else if(CommandVecMacro[0] == "run")
		{
			RunMacro(macro);
		}
	}
}

void Finances::RenameMacro(Macro* macro)
{
	string oldname;
	string newname;

	oldname = macro->Name;

	newname = ReadInMacroName();
	if(newname == oldname)
		return;
	while(Macros.find(newname) != Macros.end())
	{
		if(!AskTryAgain("A macro with the name \"" + newname + "\" already exists."))
		{
			printf("Your macro's name has not been changed.");
			return;
		}
		newname = ReadInMacroName();
	}

	macro->Name = newname;

	Macros.erase(oldname);
	Macros[newname] = macro;
}

void Macro::SelectTra()
{
	string s = "$"; // TODO - foreign
	map<unsigned long long,Tra*>::iterator mit;
	unsigned long long traid;
	Tra* tra;

	traid = ReadInTraId();

	if((mit = Tras.find(traid)) == Tras.end())
	{
		printf("Transaction/Transfer with Id %llu doesn't exist in this macro.\n",traid);
		return;
	}

	tra = mit->second;
	tra->Print(s);

	InteractWithUserTra(tra);
}

void Macro::InteractWithUserTra(Tra* tra)
{
	// TODO
}

void Macro::NewTra()
{
	// TODO
}

void Finances::RunMacro(Macro* macro)
{
	// TODO
}
