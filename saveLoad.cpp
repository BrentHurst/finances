/*************************************************
 *   G BRENT HURST
 *   saveLoad.cpp
 *   December 27, 2017 (original)
 *   May 8, 2018 (last edit)
 *
 *   #include "finances.h"
 *   #include "iogbh.h"
 *
 ************************************************/


#include "finances.h"
#include "iogbh.h"
#include <cstdio>
using namespace std;

const char c0 = '\0';
const char c1 = '\n';
const char esc = '\\';

#define PutLine PutDelimitedLine(f,c0,c1,esc,v)

string dtos(double d)
{
	char c[100];
	string s;
	sprintf(c,"%f",d);
	s = c;
	return s;
}
string itos(int i)
{
	char c[100];
	string s;
	sprintf(c,"%d",i);
	s = c;
	return s;
}
double stod(string& s)
{
	double d;
	sscanf(s.c_str(),"%lf",&d);
	return d;
}
int stoi(string& s)
{
	int i;
	sscanf(s.c_str(),"%d",&i);
	return i;
}

void Finances::Load(const string& filename)
{
}


void Finances::SaveAccounts(FILE* f)
{
	vector<string> v;

	v.push_back("ACCOUNTS");
	PutLine;

}
void Finances::SaveTransactions(FILE* f)
{

}
void Finances::SaveTransfers(FILE* f)
{

}

void Finances::Save(const string& filename)
{
	FILE* f;

	f = fopen(filename.c_str(),"w");

	SaveAccounts(f);
	SaveTransactions(f);
	SaveTransfers(f);

	fclose(f);
}
