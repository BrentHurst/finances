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
	cmdList['c']="Change Tag";
	cmdList['d']="Change Location";
	cmdList['e']="Change Earmark";
	cmdList['f']="Change To/From";
	cmdList['g']="Change Info";
	cmdList['h']="Reconcile";
	cmdList['i']="Unreconcile";
	cmdList['j']="Change Amount";
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

Transaction* GetTransactionFromUser(TransactionSet& ts)
{
	unsigned int i;
	vector<Transaction*> v;
	TransactionSet::iterator sit;

	for(sit = ts.begin(); sit != ts.end(); sit++)
		v.push_back(*(sit));

	for(i=0; i<v.size(); i++)
	{
		printf("%d: ",i);
		v[i]->Print();
	}

	do
	{
		printf("Please choose a transaction: ");
		i = ReadInt();
	}while(i>=v.size());

	return v[i];
}

int RunCommand(Transaction* t,char cmd)
{
	if(cmdList[cmd]=="")
		return 1;

	switch(cmd)
	{
		case 'a': //Delete
			return 1;
		case 'b': //Change Date
			return 1;
		case 'c': //Change Tag
			return 1;
		case 'd': //Change Location
			return 1;
		case 'e': //Change Earmark
			return 1;
		case 'f': //Change To/From
			return 1;
		case 'g': //Change Info
			return 1;
		case 'h': //Reconcile
			return 1;
		case 'i': //Unreconcile
			return 1;
		case 'j': //Change Amount
			return 1;
		case 'y': return 0;
		case 'z': return 0;
	}

	return 1;
}

void Finances::SelectTransaction(TransactionSet& ts)
{
	Transaction* t;

	t = GetTransactionFromUser(ts);

	FillCmdList();

	printf("\n");
	t->Print();
	while(RunCommand(t,GetCommand(cmdList)))
	{
		printf("\n");
		t->Print();
	}
}
