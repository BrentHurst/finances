#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <map>
#include "finances.hpp"
using namespace std;

string ReadString();
char ReadChar();
double ReadDouble();
int ReadInt();
unsigned long long ReadULL();

string ReadInType();
unsigned long long ReadInDate();
unsigned long long ReadInTraId();
string ReadInInfo();
double ReadInAmount();
string ReadInCurrency();
string ReadInMacroName();
string ReadInAccountTypeToChange(class Tra* tra);
string ReadInFlag();

string ReadInNewAccountName();
string ReadInParentAccountName();
string ReadInParentAccountName_Reparenting();
string ReadInNewTraAccount(const string& s);
string ReadInDefaultCurrency();


string ReadInFilename();

void ColorOutput(const string& s);

int AskTryAgain(string s);
int AskSetUpNewFinances();
int AskConfirmDeleteTra(class Tra* tra,const string& DefaultCurrency);
int AskConfirmDeleteAccount(class Account* acc);
int AskAddNonexistentAccount(const string& acc_n);
int AskAccurateAccount(const string& name, const string& cur, const string& par);
int AskAccurateTra(class Tra* tra, const string& DefaultCurrency);
int AskAccurateChangedTra(class Tra* tra, const string& DefaultCurrency);
int AskReconcileTra(Tra* tra,const string& DefaultCurrency);
int AskCreateTmpChildAccount(const string& parname, const string& newchildname);
int AskSureRunMacro(class Macro* m);

int IsAccountPartOfTra(class Account* acc, class Tra* tra);

string dtos_(double d);
string itos_(int i);
double stod_(string s);
int stoi_(string s);
string ulltos_(unsigned long long a);
double abs_(double f);
double Round2Decimals(double d);

class cmdcomp
{
	public:
		bool operator() (const int a,const int b);
};
/* typedef map<int,string,cmdcomp> CommandMap; */

unsigned long long GetNextValidTraId(unsigned long long Date, const map<unsigned long long, class Tra*>& Tras);
void PrintCurrencyAmount(const string& Currency, double Amount);
void InsertTraIntoMap(Tra* tra,map<unsigned long long,Tra*>& Tras);



#endif
