#include "finances.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using nlohmann::json;

typedef std::runtime_error SRE;

Finances::Finances(const string& fn)
{
	FillCmdList();

	filename = fn;
	LoadFromFile();
}

int Finances::RunCommand(int cmd)
{
	/* char c='r'; */

	if(CmdList[cmd]=="")
		return 1;

	switch(cmd)
	{
		/* case  1: f.ReadNewTransaction(1,0); return 1; */
		/* case  2: f.ReadNewTransfer(1,0); return 1; */
		/* case  3: f.ReadNewForeignTransaction(); return 1; */
		/* case  4: f.ReadTransferToForeign(); return 1; */
		/* case  5: f.ReadTransferFromForeign(); return 1; */
		/* case  6: f.ReadNewAccount(); return 1; */

		/* case 11: f.PrintTransactions(); return 1; */
		/* case 12: f.PrintTransfers(); return 1; */
		/* case 13: f.PrintUnreconciledTransactions(); return 1; */
		/* case 14: f.PrintUnreconciledTransfers(); return 1; */
		/* case 15: f.PrintForeignTransactions(); return 1; */
		/* case 16: f.PrintForeignTransfers(); return 1; */

		/* case 21: f.PrintAllAccounts(); return 1; */
		/* case 22: f.PrintEarmarks(); return 1; */
		/* case 23: f.PrintLocations(); return 1; */
		/* case 24: f.PrintTags(); return 1; */
		/* case 25: f.PrintTofroms(); return 1; */

		/* case 31: f.SelectAccount(); return 1; */
		/* case 32: f.SelectTransaction(f.transactions); return 1; */
		/* case 33: f.SelectTransfer(f.transfers); return 1; */

		/* case 41: f.Macros(); return 1; */

		/* case 51: f.Reconcile(); return 1; */

		/* case 61: f.NewYear(); return 1; */

		/* case  8: f.Save(); return 1; */
		/* case  9: f.Save(); return 0; */
		/* case  0:
				  while(c!='y' && c!='n')
				  {
					  printf("Are you sure you want to quit without saving? [y/n]: ");
					  c = ReadChar();
				  }
				  if(c=='y') return 0;
				  else return 1; */
	}
	return 1;
}

void Finances::Run()
{
	while(RunCommand(ReadInCommand()));
}


void Finances::FillCmdList()
{
	CmdList.clear();

	CmdList[1]="New Transaction";
	/* CmdList[2]="New Transfer"; */
	/* CmdList[3]="New Foreign Transaction"; */
	/* CmdList[4]="New Transfer to Foreign Currency Account"; */
	/* CmdList[5]="New Transfer from Foreign Currency Account"; */
	/* CmdList[6]="New Account (earmark/location/tag/tofrom)"; */

	/* CmdList[11]="List Transactions"; */
	/* CmdList[12]="List Transfers"; */
	/* CmdList[13]="List Unreconciled Transactions"; */
	/* CmdList[14]="List Unreconciled Transfers"; */
	/* CmdList[15]="List Foreign Transactions"; */
	/* CmdList[16]="List Foreign Transfers"; */

	/* CmdList[21]="List All Accounts"; */
	/* CmdList[22]="List Earmarks"; */
	/* CmdList[23]="List Locations"; */
	/* CmdList[24]="List Tags"; */
	/* CmdList[25]="List To/Froms"; */

	/* CmdList[31]="Select Account (earmark/location/tag/tofrom)"; */
	/* CmdList[32]="Select Transaction"; */
	/* CmdList[33]="Select Transfer"; */

	/* CmdList[41]="Macros"; */

	/* CmdList[51]="Reconcile"; */

	/* CmdList[61]="Start New Year from Here"; */

	/* CmdList[8]="Save"; */
	/* CmdList[9]="Save and Quit"; */
	/* CmdList[0]="Quit WITHOUT Saving"; */
}

int Finances::ReadInCommand()
{
	int i;

	PrintCommands();

	do
	{
		printf("Please choose a command: ");
		i = ReadInt();
	}while(CmdList.find(i) == CmdList.end());

	printf("\n");

	return i;
}

void Finances::PrintCommands()
{
	map<int,string,cmdcomp>::iterator mit;

	printf("\n");
	for(mit=CmdList.begin(); mit != CmdList.end(); mit++)
		if(mit->second != "")
			printf("%2d. %s\n",mit->first,mit->second.c_str());
}

void Finances::LoadFromFile()
{
	ifstream ifs;
	json j;

	ifs.clear();
	ifs.open(filename, ifstream::in);
	if(!ifs.is_open())
		throw SRE("Finances::LoadFromFile(): Couldn't open file " + filename);

	try
	{
		ifs >> j;
	}
	catch(...)
	{
		throw SRE("Finances::LoadFromFile(): Couldn't parse json from file " + filename);
	}

	ifs.close();

	FromJson(j);
}
void Finances::SaveToFile()
{
	// TODO
}
void Finances::FromJson(const json& j)
{
	cout << j.dump(4) << endl;
}
json Finances::ToJson()
{
	json j;

	// TODO

	return j;
}
