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
#include <cstdio>
using namespace std;


static map<char,string> cmdList;

static void FillCmdList()
{
	cmdList.clear();
	cmdList['a']="List Macros";
	cmdList['b']="Run a Macro";
	cmdList['c']="Add a Macro";
	cmdList['d']="Delete a Macro";
	cmdList['e']="";
	cmdList['f']="";
	cmdList['g']="";
	cmdList['h']="";
	cmdList['i']="";
	cmdList['j']="";
	cmdList['k']="";
	cmdList['l']="";
	cmdList['m']="";
	cmdList['n']="";
	cmdList['o']="";
	cmdList['p']="";
	cmdList['q']="";
	cmdList['r']="";
	cmdList['s']="";
	cmdList['t']="";
	cmdList['u']="";
	cmdList['v']="";
	cmdList['w']="";
	cmdList['x']="";
	cmdList['y']="Return to Main Menu";
	cmdList['z']="Return to Main Menu";
}

void Finances::PrintMacros()
{
	set<string>::iterator sit;
	TransactionSet::iterator tsit1;
	TransferSet::iterator tsit2;

	if(macronames.empty())
	{
		printf("There aren't any macros.\n");
		return;
	}

	for(sit=macronames.begin(); sit != macronames.end(); sit++)
	{
		printf("%s\n",(*sit).c_str());

		for(tsit1 = macrotransactions[*sit].begin(); tsit1 != macrotransactions[*sit].end(); tsit1++)
			(*tsit1)->Print();
		for(tsit2 = macrotransfers[*sit].begin(); tsit2 != macrotransfers[*sit].end(); tsit2++)
			(*tsit2)->Print();

		printf("\n");
	}

}

void Finances::RunAMacro()
{
	Date* d;
	string s;
	char c;
	TransactionSet::iterator tsit1;
	TransferSet::iterator tsit2;
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

	for(tsit2=macrotransfers[s].begin(); tsit2 != macrotransfers[s].end(); tsit2++)
	{
		t2 = (*tsit2)->Copy();
		t2->date->setWithTotalDay(d->getTotalDay());
		LinkTransfer(t2,0);
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
	TransferSet ts2;
	TransactionSet::iterator tsit1;
	TransferSet::iterator tsit2;

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
		ts2.insert(transfer);
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
	TransferSet::iterator tsit2;

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

	for(tsit2 = macrotransfers[s].begin(); tsit2 != macrotransfers[s].end(); tsit2++)
		delete *tsit2;
	macrotransfers.erase(macrotransfers.find(s));
}

int RunCommand(Finances* f,char cmd)
{
	if(cmdList[cmd]=="")
		return 1;

	switch(cmd)
	{
		case 'a': f->PrintMacros(); return 1;
		case 'b': f->RunAMacro(); return 1;
		case 'c': f->AddAMacro(); return 1;
		case 'd': f->DeleteAMacro(); return 1;
		case 'y': return 0;
		case 'z': return 0;
	}

	return 1;
}

void Finances::Macros()
{
	FillCmdList();

	while(RunCommand(this,GetCommand(cmdList)));
}
