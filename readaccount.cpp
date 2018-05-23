#include "finances.h"
#include "readtype.h"
#include "ask.h"
#include "number.h"
using namespace std;


void Finances::FindSuperAccount(const string& str,Account* a,AccountMap& m,string type,int setup)
{
	char c='r';

	while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
	{
		printf("Does %s have a superaccount? [y/n]: ",str.c_str());
		c = ReadChar();
	}
	if(c=='n' || c=='N') return;

	PrintCorrectAccountMap(type);
	printf("Please enter superaccount name. ");
	a->superaccount = ReadInAccount(m,type,1,setup);
	if(a->superaccount)
		a->superaccount->subaccounts.insert(make_pair(a->name,a));
}

Account* Finances::ReadInAccount(AccountMap& m,string type,int z,int setup)
{
	string str;
	Account* a;
	AccountMap::iterator mit;
	double d;

	while(1)
	{
		PrintCorrectAccountMap(type);
		printf("%s: ",type.c_str());
		str = ReadString();

		//str not in m
		if((mit=m.find(str))==m.end())
		{
			//str in another account map
			if(allaccounts.find(str) != allaccounts.end())
			{
				printf("\"%s\" already exists as a %s.\n",str.c_str(),(*allaccounts.find(str)).second->t.c_str());
				if(!AskForContinue()) return 0;
				else continue;
			}
			//str doesn't exist yet as an account
			else
			{
				PrintCorrectAccountMap(type);
				printf("\n");
				if(AskToAdd(type,str))
				{
					a = new Account(str,type,currency);
					if(setup)
					{
						printf("Enter the starting amount in this %s: %s",type.c_str(),currency.c_str());
						d = ReadDouble();
						a->amount = Round2Decimals(d);
					}
					m[str] = a;
					allaccounts[str] = a;

					FindSuperAccount(str,a,m,type,setup);

					return a;
				}
				else
				{
					if(!AskForContinue()) return 0;
					else continue;
				}
			}
		}

		//str in m
		else
		{
			if(!z)
				printf("\"%s\" already exists.\n",str.c_str());
			return mit->second;
		}
	}
}



void Finances::ReadNewAccount()
{
	char c='r';

	printf("1. earmark\n2. location\n3. tag\n4. to/from\n");
	while(c<'1' || c>'4')
	{
		printf("Please enter the number of the type of account you are creating: ");
		c = ReadChar();
	}
	switch(c)
	{
		case '1': ReadInAccount(earmarks,"earmark",0,0); break;
		case '2': ReadInAccount(locations,"location",0,0); break;
		case '3': ReadInAccount(tags,"tag",0,0); break;
		case '4': ReadInAccount(tofroms,"to/from",0,0); break;
	}
}

void Finances::RenameAccount(Account* a)
{
	string s;
	char c;

	while(1)
	{
		printf("Please enter what you would like to rename %s to: ",a->name.c_str());
		s = ReadString();
		if(allaccounts.find(s)==allaccounts.end())
			break;
		do
		{
			printf("\"%s\" already exists. Try again? [y/n]: ",s.c_str());
			c = ReadChar();
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='n' || c=='N')
			return;
	}

	allaccounts[s] = a;
	allaccounts.erase(allaccounts.find(a->name));

	if(a->superaccount)
	{
		a->superaccount->subaccounts[s] = a;
		a->superaccount->subaccounts.erase(a->superaccount->subaccounts.find(a->name));
	}

	switch(a->type)
	{
		case location:
					   locations[s] = a;
					   locations.erase(locations.find(a->name));
					   break;
		case earmark:
					   earmarks[s] = a;
					   earmarks.erase(earmarks.find(a->name));
					   break;
		case tag:
					   tags[s] = a;
					   tags.erase(tags.find(a->name));
					   break;
		case tofrom:
					   tofroms[s] = a;
					   tofroms.erase(tofroms.find(a->name));
					   break;
	}

	a->name = s;
}
