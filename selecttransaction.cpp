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

static void Reconcile(Transaction* t,int i)
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

static void ChangeDate(Transaction* t)
{
	delete t->date;
	t->date = new Date;
	t->date->ReadInDate();
}

static void Delete(Finances* f,Transaction* t)
{
	char c;

	do
	{
		t->Print();
		printf("Are you sure you want to permanently delete this transaction? [y/n]: ");
		c = ReadChar();
	}while(c!='y' && c!='Y' && c!='n' && c!='N');

	if(c=='y' || c=='Y')
	{
		f->UnlinkTransaction(t);
		delete t->date;
		delete t;
		printf("This transaction has been deleted.\n");
	}
	else
		printf("This transaction hasn't been deleted.\n");
}

int RunCommand(Finances* f,Transaction* t,char cmd)
{
	if(cmdList[cmd]=="")
		return 1;

	switch(cmd)
	{
		case 'a': Delete(f,t); return 1;
		case 'b': ChangeDate(t); return 1;
		case 'c': //Change Tag
			return 1;
		case 'd': //Change Location
			return 1;
		case 'e': //Change Earmark
			return 1;
		case 'f': //Change To/From
			return 1;
		case 'g': t->info = ReadInInformation(); return 1;
		case 'h': Reconcile(t,1); return 1;
		case 'i': Reconcile(t,0); return 1;
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
	while(RunCommand(this,t,GetCommand(cmdList)))
	{
		printf("\n");
		t->Print();
	}
}
