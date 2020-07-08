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
	cin.ignore();
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
