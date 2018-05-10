/*************************************************
 *   G BRENT HURST
 *   setup.cpp
 *   May 9, 2018 (original)
 *   May 10, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
using namespace std;

void Finances::SetupAddAccounts(const string& type,map<string,Account*>& m)
{
	char c;
	char s[100];
	string str;
	double d;
	Account* a;

	while(1)
	{
		if(!m.empty())
		{
			do
			{
				printf("Enter another %s? [y/n]: ",type.c_str());
				scanf("%c\n",&c);
			}while(c!='y' && c!='Y' && c!='n' && c!='N');

			if(c=='n' || c=='N')
				break;
		}

		printf("Enter a %s name: ",type.c_str());
		scanf("%s\n",s);
		printf("Enter the starting amount in this %s: $",type.c_str());
		scanf("%lf\n",&d);

		do
		{
			printf("Is \"%s\" with $%f correct? [y/n]: ",s,d);
			scanf("%c\n",&c);
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='y' || c=='Y')
		{
			str = s;
			a = new Account(str,type);
			a->amount = d;
			m[str] = a;
			allaccounts[str] = a;
			FindSuperAccount(str,a,m,type);
		}
	}
}

void Finances::Setup(const string& filename)
{
	char c;

	SetupAddAccounts("location",locations);
	SetupAddAccounts("earmark",earmarks);

	do
	{
		printf("Would you like to save changes or discard changes?");
		printf("Enter 1 for save, and enter 2 for discard: ");
		scanf("%c\n",&c);
	}while(c!='1' && c!='2');

	if(c=='1')
		Save(filename);
	else
		exit(0);
}
