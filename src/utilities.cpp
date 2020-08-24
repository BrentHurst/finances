#include <cstdlib>
#include <string>
#include <cstdio>
#include <iostream>
#include <cmath>
#include "utilities.hpp"

using namespace std;

string ReadString()
{
	string s;
	getline(cin,s);
	return s;
}

char ReadChar()
{
	char c;
	scanf("%c",&c);
	if(c != '\n')
		cin.ignore();
	return c;
}

double ReadDouble()
{
	double d;
	scanf("%lf",&d);
	cin.ignore();
	return d;
}

int ReadInt()
{
	int i;
	scanf("%d",&i);
	cin.ignore();
	return i;
}

unsigned long long ReadULL()
{
	unsigned long long a;
	scanf("%llu",&a);
	cin.ignore();
	return a;
}

string ReadInInfo()
{
	printf("info: ");
	return ReadString();
}

double ReadInAmount()
{
	printf("Amount: (positive for income or transfer, negative for spent): ");
	return Round2Decimals(ReadDouble());
}

string ReadInNewAccountName()
{
	printf("New Account Name: ");
	return ReadString();
}

string ReadInNewTraAccount(const string& s)
{
	printf("%s: ",s.c_str());
	return ReadString();
}

string ReadInType()
{
	int i = 0;
	do
	{
		printf("Transaction or Transfer? 1 for Transaction, 2 for Transfer. [1/2]: ");
		i = ReadInt();
	}while(i != 1 && i != 2);
	return (i == 1) ? "Transaction" : "Transfer";
}

unsigned long long ReadInDate()
{
	printf("Date (integer): ");
	return ReadULL();
}

unsigned long long ReadInTraId()
{
	printf("Id: ");
	return ReadULL();
}

string ReadInCurrency()
{
	printf("Currency: ");
	return ReadString();
}

string ReadInParentAccountName()
{
	printf("New Account's Parent Account: ");
	return ReadString();
}

string ReadInParentAccountName_Reparenting()
{
	string s;
	do
	{
		printf("Account's New Parent Account: ");
		s = ReadString();
	}while(!s.size());
	return s;
}

string ReadInAccountTypeToChange(Tra* tra)
{
	// TODO - something with currency
	string s;

	do
	{
		printf("What attribute of this %s would you like to change?\n",tra->Type.c_str());
		printf("Options: Tag, Location, Earmark, ToFrom, From, To, Date, Info, Amount.\n");
		printf("Enter q to quit.\n");
		s = ReadString();
	}while(s != "Tag" && s != "Location" && s != "Earmark" && s != "ToFrom" && s != "From" && s != "To" && s != "Date" && s != "Info" && s != "Amount" && s != "q");

	return s;
}

string ReadInFlag()
{
	string s;

	do
	{
		printf("Flag: ");
		s = ReadString();
	}while(!s.size());

	return s;
}

static int FileExists(const string& name)
{
	FILE* file;
	if((file = fopen(name.c_str(), "r")))
	{
		fclose(file);
		return 1;
	}
	else
	{
		return 0;
	}
}

string ReadInFilename()
{
	string s;
	string OutMessage = "Please enter a filename";

	printf("It's recommended that your filename use the extension .fin, but this is not required.\n");
	printf("%s: ",OutMessage.c_str());
	s = ReadString();
	while(!s.size() || FileExists(s))
	{
		if(s.size())
		{
			if(!AskTryAgain("File \"" + s + "\" already exists."))
				return "";
		}
		printf("%s: ",OutMessage.c_str());
		s = ReadString();
	}

	return s;
}

string ReadInDefaultCurrency()
{
	string s;
	char c;
	do
	{
		printf("Default Currency: ");
		s = ReadString();
		do
		{
			printf("You entered \"%s\" as your default currency symbol. Is that correct? [y/n]: ",s.c_str());
			c = ReadChar();
		}while(c != 'y' && c != 'n');
	}while(c != 'y');
	return s;
}


void ColorOutput(const string& s)
{
	if(s == "Red")
		printf("\033[0;31m");
	else if(s == "Green")
		printf("\033[0;32m");
	else if(s == "Yellow")
		printf("\033[0;33m");
	else if(s == "Blue")
		printf("\033[0;34m");
	else if(s == "Magenta")
		printf("\033[0;35m");
	else if(s == "Cyan")
		printf("\033[0;36m");
	else if(s == "Bold Red")
		printf("\033[1;31m");
	else if(s == "Bold Green")
		printf("\033[1;32m");
	else if(s == "Bold Yellow")
		printf("\033[1;33m");
	else if(s == "Bold Blue")
		printf("\033[1;34m");
	else if(s == "Bold Magenta")
		printf("\033[1;35m");
	else if(s == "Bold Cyan")
		printf("\033[1;36m");
	else if(s == "Reset" || s == " " || s == "")
		printf("\033[0m");
}

int AskTryAgain(string s)
{
	char c;

	if(s.size())
		printf("%s\n",s.c_str());

	do
	{
		printf("Would you like to try again? [y/n]: ");
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskSetUpNewFinances()
{
	char c;

	do
	{
		printf("Would you like to set up a new finances file? [y/n]: ");
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskConfirmDeleteTra(Tra* tra, const string& DefaultCurrency)
{
	char c;

	tra->Print(DefaultCurrency);

	do
	{
		printf("Are you sure you want to delete this %s? [y/n]: ",tra->Type.c_str());
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskConfirmDeleteAccount(Account* acc)
{
	char c;

	acc->Print("\t");

	do
	{
		printf("Are you sure you want to delete %s with name \"%s\"? [y/n]: ",acc->Type.c_str(),acc->Name.c_str());
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskAddNonexistentAccount(const string& acc_n)
{
	char c;

	printf("There is no account with the name \"%s\".\n",acc_n.c_str());

	do
	{
		printf("Would you like to add it? [y/n]: ");
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskAccurateAccount(const string& name, const string& cur, const string& par)
{
	char c;

	printf("\tAccount:  %s\n",name.c_str());
	printf("\tCurrency: %s\n",cur.c_str());
	printf("\tParent:   %s\n",par.c_str());

	do
	{
		printf("Is the above new account information correct? [y/n]: ");
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskAccurateTra(Tra* tra, const string& DefaultCurrency)
{
	char c;

	tra->Print(DefaultCurrency);

	do
	{
		printf("Is the above %s correct? [y/n]: ",tra->Type.c_str());
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskAccurateChangedTra(Tra* tra, const string& DefaultCurrency)
{
	char c;

	tra->Print(DefaultCurrency);

	do
	{
		printf("If the above %s is not correct, it will be undone.\n",tra->Type.c_str());
		printf("Is the above %s correct? [y/n]: ",tra->Type.c_str());
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskReconcileTra(Tra* tra,const string& DefaultCurrency)
{
	char c;

	printf("\n\n");
	tra->Print(DefaultCurrency);

	do
	{
		printf("Would you like to reconcile the above %s? [y/n]: ",tra->Type.c_str());
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int AskCreateTmpChildAccount(const string& parname, const string& newchildname)
{
	char c;

	do
	{
		printf("Account \"%s\" has a nonzero amount and/or tras associated with it.\n",parname.c_str());
		printf("A new account \"%s\" will be created as a child of \"%s\" and will take over its tras and amount.\n",newchildname.c_str(),parname.c_str());
		printf("(The new account's name can be changed later.)\n");
		printf("Would you like to continue and allow this to happen? [y/n]: ");
		c = ReadChar();
	}while(c != 'y' && c != 'n');

	return (c == 'y');
}

int IsAccountPartOfTra(Account* acc, Tra* tra)
{
	Account* target;

	if(!acc || !tra)
		return 0;

	if(tra->Type == "Transaction")
	{
		if(acc->Type == "Tag")
			target = tra->Tag;
		else if(acc->Type == "Location")
			target = tra->Location;
		else if(acc->Type == "Earmark")
			target = tra->Earmark;
		else if(acc->Type == "ToFrom")
			target = tra->ToFrom;

		for( ; target; target = target->Parent)
			if(target == acc)
				return 1;

		return 0;
	}
	else if(tra->Type == "Transfer")
	{
		for(target = tra->From; target; target = target->Parent)
			if(target == acc)
				return 1;

		for(target = tra->To; target; target = target->Parent)
			if(target == acc)
				return 1;

		return 0;
	}

	return 0;
}


string dtos_(double d)
{
	char c[100];
	string s;
	sprintf(c,"%.2f",d);
	s = c;
	return s;
}
string itos_(int i)
{
	char c[100];
	string s;
	sprintf(c,"%d",i);
	s = c;
	return s;
}
double stod_(string s)
{
	double d;
	sscanf(s.c_str(),"%lf",&d);
	return d;
}
int stoi_(string s)
{
	int i;
	sscanf(s.c_str(),"%d",&i);
	return i;
}
string ulltos_(unsigned long long a)
{
	char c[100];
	string s;
	sprintf(c,"%llu",a);
	s = c;
	return s;
}
double abs_(double f)
{
	return (f >= 0) ? f : -1*f;
}
double Round2Decimals(double d)
{
	double rv = round(d*100.0)/100.0;
	return (rv == -0.0) ? 0 : rv;
}


/* bool cmdcomp::operator() (int a,int b)
{
	if(b==a) return false;
	if(b==0) return true;
	if(a==0) return false;
	if(b==9) return true;
	if(a==9) return false;
	if(b==8) return true;
	if(a==8) return false;
	return (a<b);
} */

unsigned long long GetNextValidTraId(unsigned long long Date, const map<unsigned long long, class Tra*>& Tras)
{
	unsigned long long Id;

	for(Id = Date * 10000; Tras.find(Id) != Tras.end(); Id++);

	return Id;
}

void PrintCurrencyAmount(const string& Currency, double Amount)
{
	if(Amount < 0)
		ColorOutput("Bold Red");
	else if(Amount > 0)
		ColorOutput("Bold Cyan");
	else if(Amount == 0)
		ColorOutput("Cyan");
	printf("%c%s%10.2lf",((Amount < 0) ? '-' : (Amount > 0) ? '+' : ' '),Currency.c_str(),abs_(Amount));
	ColorOutput("Reset");
}

void InsertTraIntoMap(Tra* tra,map<unsigned long long,Tra*>& Tras)
{
	if(!tra->Id || Tras.find(tra->Id) != Tras.end())
		tra->Id = GetNextValidTraId(tra->Date, Tras);
	Tras[tra->Id] = tra;
}

void Finances::ReparentCP(Account* child, Account* parent)
{
	string tmpaccname;
	bool neednewkiddo;
	Account* thenewkiddo;

	if(!child || !parent)
		return;

	if(IsDeleteAccount(parent))
	{
		printf("Delete accounts can't have children.\n");
		return;
	}

	if(IsHeadAccount(child))
	{
		printf("Head accounts can't have parents.\n");
		return;
	}

	if(child->Type != parent->Type)
	{
		printf("Accounts aren't of the same type.\n");
		return;
	}

	if(child == parent)
	{
		printf("A child can't be its own parent and the fact that the code reached this point means that there's a nonzero chance it's created an issue that Brent needs to fix. Error 1082.\n");
		return;
	}

	if(!parent->Children.size())
	{
		neednewkiddo = false;

		if(parent->Amount)
		{
			neednewkiddo = true;
		}
		else
		{
			for(map<unsigned long long, Tra*>::iterator mit = Tras.begin(); mit != Tras.end(); ++mit)
			{
				if(IsAccountPartOfTra(parent,mit->second))
				{
					neednewkiddo = true;
					break;
				}
			}
		}

		if(neednewkiddo)
		{
			for(tmpaccname = parent->Name; AllAccounts.find(tmpaccname) != AllAccounts.end(); tmpaccname += "-tmp");

			if(!AskCreateTmpChildAccount(parent->Name,tmpaccname))
				return;

			thenewkiddo = new Account(tmpaccname, parent->Amount, parent->Type, parent->Currency);
			thenewkiddo->Parent = parent;
			parent->Children[thenewkiddo->Name] = thenewkiddo;

			for(map<unsigned long long, Tra*>::iterator mit = Tras.begin(); mit != Tras.end(); ++mit)
			{
				if(mit->second->Tag == parent)
					mit->second->Tag = thenewkiddo;
				if(mit->second->Location == parent)
					mit->second->Location = thenewkiddo;
				if(mit->second->Earmark == parent)
					mit->second->Earmark = thenewkiddo;
				if(mit->second->ToFrom == parent)
					mit->second->ToFrom = thenewkiddo;
				if(mit->second->From == parent)
					mit->second->From = thenewkiddo;
				if(mit->second->To == parent)
					mit->second->To = thenewkiddo;
			}
		}
	}

	// TODO - Something with foreign here probably
	for(Account* a = child->Parent; a; a = a->Parent)
		a->Amount -= child->Amount;
	for(Account* a = parent; a; a = a->Parent)
		a->Amount += child->Amount;

	if(child->Parent)
		child->Parent->Children.erase(child->Name);

	parent->Children[child->Name] = child;
	child->Parent = parent;
}
