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
#include <cstdio>
#include <cstdlib>
using namespace std;

void Finances::SetupAddAccounts(const string& type,map<string,Account*>& m)
{
	char c;
	char s[100];
	string str;
	double d;
	Account* a;
	int junk;
	int earmarkAmount;
	map<string,Account*>::iterator mit;


	while(1)
	{
		amount = 0;
		for(mit = locations.begin(); mit != locations.end(); mit++)
			if(!mit->second->superaccount)
				amount += mit->second->amount;

		earmarkAmount = 0;
		for(mit = earmarks.begin(); mit != earmarks.end(); mit++)
			if(!mit->second->superaccount)
				earmarkAmount += mit->second->amount;

		if(!m.empty() && type=="location")
		{
			do
			{
				printf("Enter another %s? [y/n]: ",type.c_str());
				scanf("%c",&c);
				FlushInputBuffer;
			}while(c!='y' && c!='Y' && c!='n' && c!='N');

			if(c=='n' || c=='N')
				break;
		}
		else if(type=="earmark" && amount==earmarkAmount)
			break;
		else if(type=="earmark" && earmarkAmount>amount)
		{
			printf("You've entered more money in earmarks than you did in accounts.\n");
			printf("Please try again.\n");
			exit(0);
		}

		printf("Enter %s name: ",type.c_str());
		scanf("%s",s);
		FlushInputBuffer;
		str = s;

		if(allaccounts.find(s) != allaccounts.end())
		{
			printf("\"%s\" already exists as a %s.\n",s,(*allaccounts.find(str)).second->t.c_str());
			continue;
		}

		printf("Enter the starting amount in this %s: $",type.c_str());
		scanf("%lf",&d);
		FlushInputBuffer;

		do
		{
			printf("Is \"%s\" with $%9.2f correct? [y/n]: ",s,d);
			scanf("%c",&c);
			FlushInputBuffer;
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='y' || c=='Y')
		{
			a = new Account(str,type);
			a->amount = d;
			m[str] = a;
			allaccounts[str] = a;
			FindSuperAccount(str,a,m,type,1);
		}
	}
}

void Finances::Setup(const string& filename)
{
	char c;
	int junk;

	SetupAddAccounts("location",locations);
	SetupAddAccounts("earmark",earmarks);

	do
	{
		printf("Would you like to save changes or discard changes?\n");
		printf("Enter 1 for save, and enter 2 for discard: ");
		scanf("%c",&c);
		FlushInputBuffer;
	}while(c!='1' && c!='2');

	if(c=='1')
		Save(filename);
	else
		exit(0);
}
