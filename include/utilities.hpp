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

string ReadInInformation();
double ReadInTotal();
string ReadInNewAccountName();
string ReadInCurrency();
string ReadInParentAccountName();

int AskTryAgain(string s);
int AskAccurateAccount(const string& name, const string& cur, const string& par);

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

#endif
