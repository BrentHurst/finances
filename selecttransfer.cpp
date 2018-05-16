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
#include <vector>
#include <cstdio>
using namespace std;

static map<char,string> cmdList;

static void FillCmdList()
{
	cmdList.clear();
	cmdList['a']="Delete";
	cmdList['b']="Change Date";
	cmdList['c']="Change From";
	cmdList['d']="Change To";
	cmdList['e']="Change Info";
	cmdList['f']="Reconcile";
	cmdList['g']="Unreconcile";
	cmdList['h']="Change Amount";
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

Transfer* GetTransferFromUser(TransferSet& ts)
{
	unsigned int i;
	vector<Transfer*> v;
	TransferSet::iterator sit;

	for(sit = ts.begin(); sit != ts.end(); sit++)
		v.push_back(*(sit));

	for(i=0; i<v.size(); i++)
	{
		printf("%d: ",i);
		v[i]->Print();
	}

	do
	{
		printf("Please choose a transfer: ");
		i = ReadInt();
	}while(i>=v.size());

	return v[i];
}

static void Reconcile(Transfer* t,int i)
{
	if(t->reconciled == i)
	{
		printf("You set it to what it already was.\n");
		return;
	}
	else
	{
		t->reconciled = i;
		printf("\nSave and quit for your change to this transaction to take full effect.\n\n");
	}
}

static void ChangeDate(Transfer* t)
{
	delete t->date;
	t->date = new Date;
	t->date->ReadInDate();
}

static void Delete(Finances* f,Transfer* t)
{
	char c;

	do
	{
		t->Print();
		printf("Are you sure you want to permanently delete this transfer? [y/n]: ");
		c = ReadChar();
	}while(c!='y' && c!='Y' && c!='n' && c!='N');

	if(c=='y' || c=='Y')
	{
		f->UnlinkTransfer(t);
		delete t->date;
		delete t;
		printf("This transfer has been deleted.\n");
	}
	else
		printf("This transfer hasn't been deleted.\n");
}

int RunCommand(Finances* f,Transfer* t,char cmd)
{
	if(cmdList[cmd]=="")
		return 1;

	switch(cmd)
	{
		case 'a': Delete(f,t); return 1; return 1;
		case 'b': ChangeDate(t); return 1;
		case 'c': //Change From
			return 1;
		case 'd': //Change To
			return 1;
		case 'e': t->info = ReadInInformation(); return 1;
		case 'f': Reconcile(t,1); return 1;
		case 'g': Reconcile(t,0); return 1;
		case 'h': //Change Amount
			return 1;
		case 'y': return 0;
		case 'z': return 0;
	}

	return 1;
}

void Finances::SelectTransfer(TransferSet& ts)
{
	Transfer* t;

	t = GetTransferFromUser(ts);

	FillCmdList();

	printf("\n");
	t->Print();
	while(RunCommand(this,t,GetCommand(cmdList)));
	{
		printf("\n");
		t->Print();
	}
}
