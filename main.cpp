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

map<char,string> cmdList;

void FillCmdList()
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
	cmdList['j']="";
	cmdList['k']="";
	cmdList['l']="";
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

void PrintCommands()
{
	map<char,string>::iterator mit;

	for(mit=cmdList.begin(); mit != cmdList.end(); mit++)
		if(mit->second != "")
			printf("%c. %s\n",mit->first,mit->second.c_str());
}

char GetCommand()
{
	char c='1';
	int junk;

	PrintCommands();

	while(c<'a' || c>'z')
	{
		printf("Please choose a command: ");
		scanf("%c",&c);
		FlushInputBuffer;
		if(c>='A' && c<='Z')
			c += ('a' - 'A');
	}

	return c;
}

int RunCommand(Finances& f,char cmd)
{
	char c='r';
	int junk;

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
		//case 'j': break;
		//case 'k': break;
		//case 'l': break;
		//case 'm': break;
		//case 'n': break;
		//case 'o': break;
		case 'p': f.PrintAllAccounts(); return 1;
		//case 'q': break;
		case 'r': //Reconcile
				  return 1;
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
					  scanf("%c",&c);
					  FlushInputBuffer;
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

	while(RunCommand(f,GetCommand()));

	return 0;
}
