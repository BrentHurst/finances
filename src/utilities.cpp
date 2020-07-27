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
double abs_(double f)
{
	return (f >= 0) ? f : -1*f;
}
double Round2Decimals(double d)
{
	double rv = round(d*100.0)/100.0;
	return (rv == -0.0) ? 0 : rv;
}




bool cmdcomp::operator() (int a,int b)
{
	if(b==a) return false;
	if(b==0) return true;
	if(a==0) return false;
	if(b==9) return true;
	if(a==9) return false;
	if(b==8) return true;
	if(a==8) return false;
	return (a<b);
}

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
