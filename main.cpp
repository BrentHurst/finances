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

const string filename = "finfo.txt";

const char printAll = 'p';
const char reconcile = 'r';
const char save = 's';
const char quitSave = 'y';
const char quitNoSave = 'z';

static map<char,string> cmdList;

static void FillCmdList()
{
	cmdList.clear();
	cmdList['a']="New Transaction";
	cmdList['b']="New Transfer";
	cmdList['c']="New Account (earmark/location/tag/tofrom)";
	cmdList['d']="List Transactions";
	cmdList['e']="List Transfers";
	cmdList['f']="List Earmarks";
	cmdList['g']="List Locations";
	cmdList['h']="List Tags";
	cmdList['i']="List To/Froms";
	cmdList['j']="Select Account (earmark/location/tag/tofrom)";
	cmdList['k']="Select Transaction";
	cmdList['l']="Select Transfer";
	cmdList['m']="";
	cmdList['n']="";
	cmdList['o']="";
	cmdList['p']="List All Accounts";
	cmdList['q']="";
	cmdList['r']="Reconcile";
	cmdList['s']="Save";
	cmdList['t']="";
	cmdList['u']="";
	cmdList['v']="";
	cmdList['w']="";
	cmdList['x']="";
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
		case 'a': f.ReadNewTransaction(); return 1;
		case 'b': f.ReadNewTransfer(); return 1;
		case 'c': f.ReadNewAccount(); return 1;
		case 'd': f.PrintTransactions(); return 1;
		case 'e': f.PrintTransfers(); return 1;
		case 'f': f.PrintEarmarks(); return 1;
		case 'g': f.PrintLocations(); return 1;
		case 'h': f.PrintTags(); return 1;
		case 'i': f.PrintTofroms(); return 1;
		case 'j': f.SelectAccount(); return 1;
		//case 'k': Select Transaction
		//break;
		//case 'l': Select Transfer
		//break;
		//case 'm': break;
		//case 'n': break;
		//case 'o': break;
		case 'p': f.PrintAllAccounts(); return 1;
		//case 'q': break;
		case 'r': f.Reconcile(); return 1;
		case 's': f.Save(filename); return 1;
		//case 't': break;
		//case 'u': break;
		//case 'v': break;
		//case 'w': break;
		//case 'x': break;
		case 'y': f.Save(filename); return 0;
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

	f.Load(filename);

	while(RunCommand(f,GetCommand(cmdList)));

	return 0;
}
