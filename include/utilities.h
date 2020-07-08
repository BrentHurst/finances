#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
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

#endif
