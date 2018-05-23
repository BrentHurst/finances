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

static CommandMap cmdList;

static void FillCmdList()
{
	cmdList.clear();

	cmdList[1]="Change Tag";
	cmdList[2]="Change Location";
	cmdList[3]="Change Earmark";
	cmdList[4]="Change To/From";

	cmdList[11]="Change Date";
	cmdList[12]="Change Info";
	cmdList[13]="Change Amount";

	cmdList[21]="Reconcile";
	cmdList[22]="Unreconcile";

	cmdList[31]="Delete";

	cmdList[9]="Return to Main Menu";
	cmdList[0]="Return to Main Menu";
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
	}while(i>=v.size() || !v[i]);

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

static void ChangeTag(Finances* f,Transaction* t)
{
	Transaction* t2 = t->Copy();
	f->UnlinkTransaction(t);
	delete t->date;
	delete t;
	t2->tag = f->ReadInAccount(f->tags,"tag",1,0);
	f->LinkTransaction(t2,0);
	t2->Print();
}

static void ChangeLocation(Finances* f,Transaction* t)
{
	Transaction* t2 = t->Copy();
	f->UnlinkTransaction(t);
	delete t->date;
	delete t;
	t2->location = f->ReadInAccount(f->locations,"location",1,0);
	f->LinkTransaction(t2,0);
	t2->Print();
}

static void ChangeEarmark(Finances* f,Transaction* t)
{
	Transaction* t2 = t->Copy();
	f->UnlinkTransaction(t);
	delete t->date;
	delete t;
	t2->earmark = f->ReadInAccount(f->earmarks,"earmark",1,0);
	f->LinkTransaction(t2,0);
	t2->Print();
}

static void ChangeToFrom(Finances* f,Transaction* t)
{
	Transaction* t2 = t->Copy();
	f->UnlinkTransaction(t);
	delete t->date;
	delete t;
	t2->tofrom = f->ReadInAccount(f->tofroms,"to/from",1,0);
	f->LinkTransaction(t2,0);
	t2->Print();
}

static void ChangeAmount(Finances* f,Transaction* t)
{
	Transaction* t2 = t->Copy();
	f->UnlinkTransaction(t);
	delete t->date;
	delete t;
	t2->amount = ReadInTotal();
	f->LinkTransaction(t2,0);
	t2->Print();
}

int RunCommand(Finances* f,Transaction* t,int cmd)
{
	//shouldn't happend because of check in GetCommand
	if(cmdList.find(cmd)==cmdList.end())
		return 1;

	switch(cmd)
	{
		case  1: ChangeTag(f,t); return 0;
		case  2: ChangeLocation(f,t); return 0;
		case  3: ChangeEarmark(f,t); return 0;
		case  4: ChangeToFrom(f,t); return 0;

		case 11: ChangeDate(t); return 1;
		case 12: t->info = ReadInInformation(); return 1;
		case 13: ChangeAmount(f,t); return 0;

		case 21: Reconcile(t,1); return 1;
		case 22: Reconcile(t,0); return 1;

		case 31: Delete(f,t); return 0;

		case  9: return 0;
		case  0: return 0;
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
