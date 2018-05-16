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
	cmdList['g']="";
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
		case 'f': //add automatic round-up
				  return 1;
		case 'k': f->SelectTransaction(a->transactions); return 1;
		case 'l': f->SelectTransfer(a->transfers); return 1;
		case 'm': f->SelectTransaction(a->unreconciledtransactions); return 1;
		case 'n': f->SelectTransfer(a->unreconciledtransfers); return 1;
		case 'p': a->Print(""); return 1;
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
