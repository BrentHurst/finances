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

string ReadInInformation();
double ReadInTotal();

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
