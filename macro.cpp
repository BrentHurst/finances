/*************************************************
 *   G BRENT HURST
 *   macros.cpp
 *   May 18, 2018 (original)
 *   May 18, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
#include "readtype.h"
#include "cmd.h"
#include <cstdio>
using namespace std;


static CommandMap cmdList;

static void FillCmdList()
{
	cmdList.clear();
	cmdList[1]="Add a Macro";
	cmdList[11]="List Macros";
	cmdList[21]="Run a Macro";
	cmdList[31]="Delete a Macro";
	cmdList[9]="Return to Main Menu";
	cmdList[0]="Return to Main Menu";
}

void Finances::RunAMacro()
{
	Date* d;
	string s;
	char c;
	TransactionSet::iterator tsit1;
	int i;
	Transaction* t1;
	Transfer* t2;

	if(macronames.empty())
	{
		printf("There aren't any macros.\n");
		return;
	}

	while(1)
	{
		printf("Which macro would you like to run? name: ");
		s = ReadString();
		if(macronames.find(s) != macronames.end())
			break;
		do
		{
			printf("%s doesn't exist as a macro. Would you like to try again? [y/n]: ",s.c_str());
			c = ReadChar();
		}while(c!='y' && c!='Y' && c!='n' && c!='N');

		if(c=='n' || c=='N')
			return;
	}

	d = new Date;
	d->ReadInDate();

	for(tsit1=macrotransactions[s].begin(); tsit1 != macrotransactions[s].end(); tsit1++)
	{
		t1 = (*tsit1)->Copy();
		t1->date->setWithTotalDay(d->getTotalDay());
		LinkTransaction(t1,0);
	}

	for(i=0; i<macrotransfers[s].size(); i++)
	{
		if(macrotransfers[s][i])
		{
			t2 = macrotransfers[s][i]->Copy();
			t2->date->setWithTotalDay(d->getTotalDay());
			LinkTransfer(t2,0);
		}
	}

	printf("Macro successfully run.\n");
}

void Finances::AddAMacro()
{
	string s;
	char c;
	Transaction* transaction;
	Transfer* transfer;
	TransactionSet ts1;
	TransferVec ts2;
	TransactionSet::iterator tsit1;
	TransferVec::iterator tsit2;

	if(macronames.empty())
		printf("There aren't any macros yet.\n");
	else
		PrintMacros();

	while(1)
	{
		printf("What would you like to name this macro? name: ");
		s = ReadString();

		if(macronames.find(s) != macronames.end())
		{
			printf("%s already exists as a macro.\n",s.c_str());
			continue;
		}

		do
		{
			printf("Is \"%s\" correct? [y/n]: ",s.c_str());
			c = ReadChar();
		}while(c!='n' && c!='N' && c!='y' && c!='Y');

		if(c=='y' || c=='Y')
			break;

		do
		{
			printf("Try again? [y/n]: ");
			c = ReadChar();
		}while(c!='n' && c!='N' && c!='y' && c!='Y');

		if(c=='n' || c=='N')
			return;
	}

	while(1)
	{
		do
		{
			printf("Add a transaction to this macro? [y/n]: ");
			c = ReadChar();
		}while(c!='n' && c!='N' && c!='y' && c!='Y');

		if(c=='n' || c=='N')
			break;

		transaction = ReadNewTransaction(0,0);
		ts1.insert(transaction);
	}

	while(1)
	{
		do
		{
			printf("Add a transfer to this macro? [y/n]: ");
			c = ReadChar();
		}while(c!='n' && c!='N' && c!='y' && c!='Y');

		if(c=='n' || c=='N')
			break;

		transfer = ReadNewTransfer(0,0);
		PutTransferInTransferVec(transfer,ts2);
	}

	do
	{
		printf("Is this correct?\n");

		printf("%s:",s.c_str());
		for(tsit1 = ts1.begin(); tsit1 != ts1.end(); tsit1++)
			(*tsit1)->Print();
		for(tsit2 = ts2.begin(); tsit2 != ts2.end(); tsit2++)
			(*tsit2)->Print();

		printf("[y/n]: ");
		c = ReadChar();
	}while(c!='y' && c!='Y' && c!='n' && c!='N');

	if(c=='y' || c=='Y')
	{
		macronames.insert(s);
		macrotransactions[s]=ts1;
		macrotransfers[s]=ts2;
		printf("Macro created.\n");
	}
	else
	{
		for(tsit1 = ts1.begin(); tsit1 != ts1.end(); tsit1++)
		{
			delete (*tsit1)->date;
			delete *tsit1;
		}
		for(tsit2 = ts2.begin(); tsit2 != ts2.end(); tsit2++)
		{
			delete (*tsit2)->date;
			delete *tsit2;
		}
		printf("Macro discarded.\n");
	}
}

void Finances::DeleteAMacro()
{
	string s;
	TransactionSet::iterator tsit1;
	unsigned int i;

	if(macronames.empty())
	{
		printf("There aren't any macros.\n");
		return;
	}

	PrintMacros();

	printf("Which macro would you like to delete? Name: ");
	s = ReadString();

	if(macronames.find(s)==macronames.end())
	{
		printf("%s doesn't exist as a macro. Returning to macro menu.\n",s.c_str());
		return;
	}

	macronames.erase(s);

	for(tsit1 = macrotransactions[s].begin(); tsit1 != macrotransactions[s].end(); tsit1++)
		delete *tsit1;
	macrotransactions.erase(macrotransactions.find(s));

	for(i=0; i<macrotransfers[s].size(); i++)
		if(macrotransfers[s][i])
			delete macrotransfers[s][i];
	macrotransfers.erase(macrotransfers.find(s));
}

int RunCommand(Finances* f,int cmd)
{
	//should never happen because of check in GetCommand()
	if(cmdList.find(cmd)==cmdList.end())
		return 1;

	switch(cmd)
	{
		case  1: f->AddAMacro(); return 1;
		case 11: f->PrintMacros(); return 1;
		case 21: f->RunAMacro(); return 1;
		case 31: f->DeleteAMacro(); return 1;
		case 9: return 0;
		case 0: return 0;
	}

	return 1;
}

void Finances::Macros()
{
	FillCmdList();

	while(RunCommand(this,GetCommand(cmdList)));
}
