/*************************************************
 *   G BRENT HURST
 *   setup.cpp
 *   May 9, 2018 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include "readtype.h"
#include "number.h"
#include <cstdio>
#include <cstdlib>
using namespace std;

void Finances::SetupAddAccounts(const string& type,AccountMap& m)
{
	char c;
	string str;
	double d;
	Account* a;
	double earmarkAmount;
	AccountMap::iterator mit;
	string curr;
	int fgn = 0;


	while(1)
	{
		amount = 0;
		for(mit = locations.begin(); mit != locations.end(); mit++)
			if(!mit->second->superaccount)
				amount = Round2Decimals(amount + mit->second->amount);

		earmarkAmount = 0;
		for(mit = earmarks.begin(); mit != earmarks.end(); mit++)
			if(!mit->second->superaccount)
				earmarkAmount = Round2Decimals(earmarkAmount + mit->second->amount);

		printf("locations amount: %s%9.2f\n",currency.c_str(),amount);
		printf("earmarks  amount: %s%9.2f\n",currency.c_str(),earmarkAmount);

		if(!m.empty() && type=="location")
		{
			do
			{
				printf("Enter another %s? [y/n]: ",type.c_str());
				c = ReadChar();
			}while(c!='y' && c!='Y' && c!='n' && c!='N');

			if(c=='n' || c=='N')
				break;
		}
		else if(type=="earmark" && amount==earmarkAmount)
			break;
		else if(type=="earmark" && earmarkAmount>amount)
		{
			printf("You've entered more money in earmarks than you did in accounts.\n");
			printf("%f - %f = %f\n",earmarkAmount,amount,earmarkAmount-amount);
			printf("Please try again.\n");
			exit(0);
		}

		printf("Enter %s name: ",type.c_str());
		str = ReadString();

		if(allaccounts.find(str) != allaccounts.end())
		{
			printf("\"%s\" already exists as a %s.\n",str.c_str(),(*allaccounts.find(str)).second->t.c_str());
			continue;
		}

		do
		{
			printf("Is this a foreign currency account?");
			c = ReadChar();
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='y' || c=='Y')
		{
			fgn = 1;
			printf("Enter the foreign currency symbol for this account: ");
			curr = ReadString();
		}


		printf("Enter the starting amount in this %s: %s",
				type.c_str(),
				(fgn) ? curr.c_str() : currency.c_str()
			  );

		d = ReadDouble();

		do
		{
			printf("Is \"%s\" with %s%9.2f correct? [y/n]: ",
					str.c_str(),
					(fgn) ? curr.c_str() : currency.c_str(),
					d);
			c = ReadChar();
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='y' || c=='Y')
		{
			a = new Account(str,type,currency);
			a->amount = Round2Decimals(d);
			m[str] = a;
			allaccounts[str] = a;
			if(fgn)
			{
				a->foreign = 1;
				a->foreigncurrency = curr;
				a->foreignamount = Round2Decimals(a->amount);
				printf("Enter the amount of %s that this cost you: ",currency.c_str());
				a->amount = Round2Decimals(ReadDouble());
			}
			FindSuperAccount(str,a,m,type,1);
		}
	}
}

string SetupGetCurrency()
{
	string s;
	char c;

	do
	{
		printf("Enter the symbol of your default currency: ");
		s = ReadString();

		do
		{
			printf("Is %s correct? [y/n]: ",s.c_str());
			c = ReadChar();
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

	}while(c=='n' || c=='N');

	return s;
}

void Finances::Setup()
{
	char c;

	currency = SetupGetCurrency();

	SetupAddAccounts("location",locations);
	SetupAddAccounts("earmark",earmarks);

	do
	{
		printf("Would you like to save changes or discard changes?\n");
		printf("Enter 1 for save, and enter 2 for discard: ");
		c = ReadChar();
	}while(c!='1' && c!='2');

	if(c=='1')
		Save();
	else
		exit(0);
}