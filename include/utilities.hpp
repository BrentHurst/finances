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
string ReadInInfo();
double ReadInAmount();
string ReadInCurrency();

string ReadInNewAccountName();
string ReadInParentAccountName();
string ReadInNewTraAccount(const string& s);

void ColorOutput(const string& s);

int AskTryAgain(string s);
int AskAddNonexistentAccount(const string& acc_n);
int AskAccurateAccount(const string& name, const string& cur, const string& par);
int AskAccurateTra(class Tra* tra, const string& DefaultCurrency);

string dtos_(double d);
string itos_(int i);
double stod_(string s);
int stoi_(string s);
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
