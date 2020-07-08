#include "finances.hpp"

using namespace std;

Finances::Finances()
{
	FillCmdList();
}

void Finances::Run()
{
	// TODO
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
