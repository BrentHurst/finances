/*************************************************
 *   G BRENT HURST
 *   main.cpp (finances)
 *   December 28, 2017 (original)
 *   December 28, 2017 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/


#include "finances.h"
#include <vector>
#include <string>
#include <cstdio>
using namespace std;


const char printAll = 'p';
const char reconcile = 'r';
const char save = 's';
const char quitSave = 'y';
const char quitNoSave = 'z';
const string filename = "finfo.txt";

void FillCmdList(vector<string>& cmdList)
{
	cmdList.clear();
	cmdList.push_back("New Transaction");
	cmdList.push_back("New Transfer");
	cmdList.push_back("New Account");
	cmdList.push_back("New Location");
	cmdList.push_back("List Transactions");
	cmdList.push_back("List Transfers");
	cmdList.push_back("List Accounts");
	cmdList.push_back("List Locations");
	cmdList.push_back("");
	cmdList.push_back("");
	cmdList.push_back("");
	cmdList.push_back("");
	cmdList.push_back("");
	cmdList.push_back("");
	cmdList.push_back("");
}

void PrintCommands(vector<string>& cmdList)
{
	unsigned int i;

	for(i=0; i<cmdList.size(); i++)
		printf("%c: %s\n",i+'a',cmdList[i].c_str());
}

char GetCommand(vector<string>& cmdList)
{
	char c;

	PrintCommands(cmdList);


	return c;
}

int RunCommand(Finances& f,char cmd)
{

	return 0;
}

int main(int argc, char ** argv)
{
	Finances f;
	vector<string> cmdList;

	FillCmdList(cmdList);

	f.ReadIn(filename);

	while(RunCommand(f,GetCommand(cmdList)));

	return 0;
}
