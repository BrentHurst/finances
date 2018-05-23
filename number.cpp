#include "number.h"
#include <cstdio>
#include <cmath>
using namespace std;


string dtos_(double d)
{
	char c[100];
	string s;
	sprintf(c,"%f",d);
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

double abs_(double f) { return (f>=0) ? f : -1*f; }
double Round2Decimals(double d) { return round(d*100)/100; }

double FindRoundUpAmount(double d)
{
	d = abs_(d);
	return Round2Decimals(ceil(d) - d);
}
