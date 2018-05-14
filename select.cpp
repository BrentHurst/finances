/*************************************************
 *   G BRENT HURST
 *   select.cpp
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
	cmdList['e']="Delete Account";
	cmdList['f']="";
	cmdList['g']="";
	cmdList['h']="";
	cmdList['i']="";
	cmdList['j']="";
	cmdList['k']="Select Transaction";
	cmdList['l']="Select Transfer";
	cmdList['m']="";
	cmdList['n']="";
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
	cmdList['y']="";
	cmdList['z']="Return to Main Menu";
}
Account* Finances::GetAccountFromUser(map<string,Account*>& m)
{
	string s;
	char c;
	int junk;

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
			scanf("%c",&c);
			FlushInputBuffer;
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='n' || c=='N')
			return NULL;
	}
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
		case 'e': //delete acct
				  return 1;
		case 'k': //Select Transaction
				  return 1;
		case 'l':// Select Transfer
				  return 1;
		case 'p': a->Print(""); return 1;
		case 'r': f->RenameAccount(a); return 1;
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

	while(RunCommand(this,a,GetCommand(cmdList)));
}
