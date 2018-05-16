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

static map<char,string> cmdList;

static void FillCmdList()
{
	cmdList.clear();
	cmdList['a']="List Transactions";
	cmdList['b']="List Transfers";
	cmdList['c']="List Unreconciled Transactions";
	cmdList['d']="List Unreconciled Transfers";
	cmdList['e']="Reconcile";
	cmdList['f']="Add Automatic Round-Up Transfer";
	cmdList['g']="Delete Automatic Round-Up Transfer";
	cmdList['h']="";
	cmdList['i']="";
	cmdList['j']="";
	cmdList['k']="Select Transaction";
	cmdList['l']="Select Transfer";
	cmdList['m']="Select from Unreconciled Transactions";
	cmdList['n']="Select from Unreconciled Transfers";
	cmdList['o']="";
	cmdList['p']="List Account Info";
	cmdList['q']="";
	cmdList['r']="Rename";
	cmdList['s']="";
	cmdList['t']="";
	cmdList['u']="";
	cmdList['v']="";
	cmdList['w']="";
	cmdList['x']="";
	cmdList['y']="Return to Main Menu";
	cmdList['z']="Return to Main Menu";
}

Account* Finances::GetAccountFromUser(map<string,Account*>& m)
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

	printf("What percent of each round-up from %s should be transferred to %s?",a->name.c_str(),b->name.c_str());
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

int RunCommand(Finances* f,Account* a,char cmd)
{
	if(cmdList[cmd]=="")
		return 1;

	switch(cmd)
	{
		case 'a': PrintTransactionsGlobal(a->transactions); return 1;
		case 'b': PrintTransfersGlobal(a->transfers); return 1;
		case 'c': PrintTransactionsGlobal(a->unreconciledtransactions); return 1;
		case 'd': PrintTransfersGlobal(a->unreconciledtransfers); return 1;
		case 'e': a->Reconcile(); return 1;
		case 'f': f->AddRoundUp(a); return 1;
		case 'g': DeleteRoundUp(a); return 1;
		case 'k': f->SelectTransaction(a->transactions); return 1;
		case 'l': f->SelectTransfer(a->transfers); return 1;
		case 'm': f->SelectTransaction(a->unreconciledtransactions); return 1;
		case 'n': f->SelectTransfer(a->unreconciledtransfers); return 1;
		case 'p': a->PrintRoundUps(); return 1;
		case 'r': f->RenameAccount(a); return 1;
		case 'y': return 0;
		case 'z': return 0;
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
