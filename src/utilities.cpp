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
	/* cin.ignore(); */
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

string ReadInInformation()
{
	printf("info: ");
	return ReadString();
}

double ReadInTotal()
{
	printf("Amount: (positive for income or transfer, negative for spent): ");
	return ReadDouble();
}

string ReadInNewAccountName()
{
	printf("New Account Name: ");
	return ReadString();
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


int AskTryAgain(string s)
{
	char c;

	printf("%s\n",s.c_str());

	do
	{
		printf("Would you like to try again? [y/n]: ");
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
	return round(d*100.0)/100.0;
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
	printf("%c%s%10.2lf",((Amount < 0) ? '-' : (Amount > 0) ? '+' : ' '),Currency.c_str(),abs_(Amount));
}
