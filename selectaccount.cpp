/*************************************************
 *   G BRENT HURST
 *   selectaccount.cpp
 *   May 14, 2018 (original)
 *   May 14, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
#include <cstdio>
using namespace std;

static CommandMap cmdList;

static void FillCmdList()
{
	cmdList.clear();
	cmdList[1]="Add Automatic Round-Up Transfer";
	cmdList[2]="Delete Automatic Round-Up Transfer";

	cmdList[11]="List Transactions";
	cmdList[12]="List Transfers";
	cmdList[13]="List Unreconciled Transactions";
	cmdList[14]="List Unreconciled Transfers";
	cmdList[15]="List Account Info";

	cmdList[21]="Reconcile";

	cmdList[31]="Give a (Different) Superaccount";
	cmdList[32]="Rename";

	cmdList[41]="Select Transaction";
	cmdList[42]="Select Transfer";
	cmdList[43]="Select from Unreconciled Transactions";
	cmdList[44]="Select from Unreconciled Transfers";

	cmdList[9]="Return to Main Menu";
	cmdList[0]="Return to Main Menu";
}

Account* Finances::GetAccountFromUser(AccountMap& m)
{
	string s;
	char c;

	while(1)
	{
		PrintAllAccounts();

		printf("Please enter the account you would like to select: ");
		s = ReadString();
		if(m.find(s)!=m.end())
			return m[s];
		do
		{
			printf("\"%s\" doesn't exist. Try again? [y/n]: ",s.c_str());
			c = ReadChar();
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='n' || c=='N')
			return NULL;
	}
}

void Finances::AddRoundUp(Account* a)
{
	char c;
	Account* b;
	double percent;

	if(a->type==tofrom || a->type==tag)
	{
		printf("You can't transfer between to/froms or between tags.\n");
		return;
	}

	if(a->roundups.empty())
		printf("%s does not yet have any automatic round-up transfers.\n",a->name.c_str());
	else
		a->PrintRoundUps();

	do
	{
		printf("Add an automatic round-up transfer? [y/n]: ");
		c = ReadChar();
	}while(c!='y' && c!='Y' && c!='n' && c!='N');

	if(c=='n' || c=='N')
		return;

	printf("Select a %s to transfer round-up to:\n",a->t.c_str());

	b = ReadInAccount((a->type==location) ? locations : earmarks,a->t,1,0);

	printf("What percent of each round-up from %s should be transferred to %s?\n",a->name.c_str(),b->name.c_str());
	printf("Enter a percent without the %% symbol: ");
	percent = ReadDouble() / 100.0;

	a->roundups.insert(make_pair(percent,b));

	a->PrintRoundUps();
}

static void DeleteRoundUp(Account* a)
{
	multimap<double,Account*>::iterator mit;
	vector<multimap<double,Account*>::iterator> v;
	int i;
	unsigned int selection;


	for(mit = a->roundups.begin(); mit != a->roundups.end(); mit++,i++)
	{
		v.push_back(mit);
		printf("%d: %f%% -> %s\n",i,100 * mit->first,mit->second->name.c_str());
	}
	printf("%d: None\n",(int) v.size());

	do
	{
		printf("Which would you like to delete? \n");
		selection = ReadInt();
	}while(selection<0 || selection>v.size());

	if(selection==v.size())
		return;

	a->roundups.erase(v[selection]);

	a->PrintRoundUps();
}

void Finances::GiveAnotherSuperaccount(Account* a)
{
	Account* acc;
	char c;

	for(acc = a->superaccount; acc != NULL; acc = acc->superaccount)
		acc->amount = Round2Decimals(acc->amount - a->amount);
	if(a->superaccount)
		a->superaccount->subaccounts.erase(a->superaccount->subaccounts.find(a->name));

	do
	{
		printf("Does %s have a superaccount? [y/n]: ",a->name.c_str());
		c = ReadChar();
	}while(c!='y' && c!='Y' && c!='n' && c!='N');

	if(c=='n' || c=='N')
	{
		a->superaccount = NULL;
		return;
	}

	printf("What is the superaccount's name? ");

	switch(a->type)
	{
		case location:
						a->superaccount = ReadInAccount(locations,a->t,1,0);
						break;
		case earmark:
						a->superaccount = ReadInAccount(earmarks,a->t,1,0);
						break;
		case tag:
						a->superaccount = ReadInAccount(tags,a->t,1,0);
						break;
		case tofrom:
						a->superaccount = ReadInAccount(tofroms,a->t,1,0);
						break;
	}

	a->superaccount->subaccounts.insert(make_pair(a->name,a));
	for(acc = a->superaccount; acc != NULL; acc = acc->superaccount)
		acc->amount = Round2Decimals(acc->amount + a->amount);
}



int RunCommand(Finances* f,Account* a,int cmd)
{
	if(cmdList.find(cmd)==cmdList.end())
		return 1;

	switch(cmd)
	{
		case  1: f->AddRoundUp(a); return 1;
		case  2: DeleteRoundUp(a); return 1;

		case 11: PrintTransactionsGlobal(a->transactions); return 1;
		case 12: PrintTransfersGlobal(a->transfers); return 1;
		case 13: PrintTransactionsGlobal(a->unreconciledtransactions); return 1;
		case 14: PrintTransfersGlobal(a->unreconciledtransfers); return 1;
		case 15: a->PrintRoundUps(); return 1;

		case 21: a->Reconcile(); return 1;

		case 31: f->GiveAnotherSuperaccount(a); return 1;
		case 32: f->RenameAccount(a); return 1;

		case 41: f->SelectTransaction(a->transactions); return 1;
		case 42: f->SelectTransfer(a->transfers); return 1;
		case 43: f->SelectTransaction(a->unreconciledtransactions); return 1;
		case 44: f->SelectTransfer(a->unreconciledtransfers); return 1;

		case 9: return 0;
		case 0: return 0;
	}
	return 1;
}

void Finances::SelectAccount()
{
	Account* a;

	if(!(a = GetAccountFromUser(allaccounts)))
		return;
	FillCmdList();

	while(RunCommand(this,a,GetCommand(cmdList)))
		printf("\nAccount: %s",a->name.c_str());
}
