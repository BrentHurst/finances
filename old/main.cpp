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
#include "readtype.h"
#include "cmd.h"
#include <vector>
#include <string>
#include <cstdio>
using namespace std;

static CommandMap cmdList;

static void FillCmdList()
{
	cmdList.clear();

	cmdList[1]="New Transaction";
	cmdList[2]="New Transfer";
	cmdList[3]="New Foreign Transaction";
	cmdList[4]="New Transfer to Foreign Currency Account";
	cmdList[5]="New Transfer from Foreign Currency Account";
	cmdList[6]="New Account (earmark/location/tag/tofrom)";

	cmdList[11]="List Transactions";
	cmdList[12]="List Transfers";
	cmdList[13]="List Unreconciled Transactions";
	cmdList[14]="List Unreconciled Transfers";
	cmdList[15]="List Foreign Transactions";
	cmdList[16]="List Foreign Transfers";

	cmdList[21]="List All Accounts";
	cmdList[22]="List Earmarks";
	cmdList[23]="List Locations";
	cmdList[24]="List Tags";
	cmdList[25]="List To/Froms";

	cmdList[31]="Select Account (earmark/location/tag/tofrom)";
	cmdList[32]="Select Transaction";
	cmdList[33]="Select Transfer";

	cmdList[41]="Macros";

	cmdList[51]="Reconcile";

	cmdList[61]="Start New Year from Here";

	cmdList[8]="Save";
	cmdList[9]="Save and Quit";
	cmdList[0]="Quit WITHOUT Saving";
}

int RunCommand(Finances& f,int cmd)
{
	char c='r';

	if(cmdList[cmd]=="")
		return 0;

	switch(cmd)
	{
		case  1: f.ReadNewTransaction(1,0); return 1;
		case  2: f.ReadNewTransfer(1,0); return 1;
		case  3: f.ReadNewForeignTransaction(); return 1;
		case  4: f.ReadTransferToForeign(); return 1;
		case  5: f.ReadTransferFromForeign(); return 1;
		case  6: f.ReadNewAccount(); return 1;

		case 11: f.PrintTransactions(); return 1;
		case 12: f.PrintTransfers(); return 1;
		case 13: f.PrintUnreconciledTransactions(); return 1;
		case 14: f.PrintUnreconciledTransfers(); return 1;
		case 15: f.PrintForeignTransactions(); return 1;
		case 16: f.PrintForeignTransfers(); return 1;

		case 21: f.PrintAllAccounts(); return 1;
		case 22: f.PrintEarmarks(); return 1;
		case 23: f.PrintLocations(); return 1;
		case 24: f.PrintTags(); return 1;
		case 25: f.PrintTofroms(); return 1;

		case 31: f.SelectAccount(); return 1;
		case 32: f.SelectTransaction(f.transactions); return 1;
		case 33: f.SelectTransfer(f.transfers); return 1;

		case 41: f.Macros(); return 1;

		case 51: f.Reconcile(); return 1;

		case 61: f.NewYear(); return 1;

		case  8: f.Save(); return 1;
		case  9: f.Save(); return 0;
		case  0:
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
