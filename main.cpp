/*************************************************
 *   G BRENT HURST
 *   main.cpp (finances)
 *   December 28, 2017 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include <vector>
#include <string>
#include <cstdio>
using namespace std;

static map<char,string> cmdList;

static void FillCmdList()
{
	cmdList.clear();
	cmdList['a']="New Transaction";
	cmdList['b']="New Transfer";
	cmdList['c']="New Account (earmark/location/tag/tofrom)";
	cmdList['d']="List Transactions";
	cmdList['e']="List Transfers";
	cmdList['f']="List Unreconciled Transactions";
	cmdList['g']="List Unreconciled Transfers";
	cmdList['h']="List Earmarks";
	cmdList['i']="List Locations";
	cmdList['j']="List Tags";
	cmdList['k']="List To/Froms";
	cmdList['l']="Select Account (earmark/location/tag/tofrom)";
	cmdList['m']="Select Transaction";
	cmdList['n']="Select Transfer";
	cmdList['o']="Macros";
	cmdList['p']="List All Accounts";
	cmdList['q']="New Foreign Transaction";
	cmdList['r']="Reconcile";
	cmdList['s']="Save";
	cmdList['t']="New Transfer to Foreign Currency Account";
	cmdList['u']="New Transfer to Default Currency Account";
	cmdList['v']="List Foreign Transaction";
	cmdList['w']="List Foreign Transfers";
	cmdList['x']="Start New Year from Here";
	cmdList['y']="Save and Quit";
	cmdList['z']="Quit WITHOUT Saving";
}

int RunCommand(Finances& f,char cmd)
{
	char c='r';

	if(cmdList[cmd]=="")
		return 0;

	switch(cmd)
	{
		case 'a': f.ReadNewTransaction(1,0); return 1;
		case 'b': f.ReadNewTransfer(1,0); return 1;
		case 'c': f.ReadNewAccount(); return 1;
		case 'd': f.PrintTransactions(); return 1;
		case 'e': f.PrintTransfers(); return 1;
		case 'f': f.PrintUnreconciledTransactions(); return 1;
		case 'g': f.PrintUnreconciledTransfers(); return 1;
		case 'h': f.PrintEarmarks(); return 1;
		case 'i': f.PrintLocations(); return 1;
		case 'j': f.PrintTags(); return 1;
		case 'k': f.PrintTofroms(); return 1;
		case 'l': f.SelectAccount(); return 1;
		case 'm': f.SelectTransaction(f.transactions); return 1;
		case 'n': f.SelectTransfer(f.transfers); return 1;
		case 'o': f.Macros(); return 1;
		case 'p': f.PrintAllAccounts(); return 1;
		case 'q': f.ReadNewForeignTransaction(); return 1;
		case 'r': f.Reconcile(); return 1;
		case 's': f.Save(); return 1;
		case 't': f.ReadTransferToForeign(); return 1;
		case 'u': f.ReadTransferFromForeign(); return 1;
		case 'v': f.PrintForeignTransactions(); return 1;
		case 'w': f.PrintForeignTransfers(); return 1;
		case 'x': f.NewYear(); return 1;
		case 'y': f.Save(); return 0;
		case 'z':
				  while(c!='y' && c!='n')
				  {
					  printf("Are you sure you want to quit without saving? [y/n]: ");
					  c = ReadChar();
				  }
				  if(c=='y') return 0;
				  else return 1;
	}
	return 1;
}

int main(int argc, char ** argv)
{
	Finances f;

	FillCmdList();

	f.Load();

	while(RunCommand(f,GetCommand(cmdList)));

	return 0;
}
