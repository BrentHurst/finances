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

int RunCommand(Transfer* t,char cmd)
{
	if(cmdList[cmd]=="")
		return 1;

	switch(cmd)
	{
		case 'a': //Delete
			return 1;
		case 'b': //Change Date
			return 1;
		case 'c': //Change From
			return 1;
		case 'd': //Change To
			return 1;
		case 'e': //Change Info
			return 1;
		case 'f': //Reconcile
			return 1;
		case 'g': //Unreconcile
			return 1;
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
	while(RunCommand(t,GetCommand(cmdList)));
	{
		printf("\n");
		t->Print();
	}
}
