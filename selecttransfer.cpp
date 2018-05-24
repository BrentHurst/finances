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
#include "readtype.h"
#include "cmd.h"
#include <vector>
#include <cstdio>
using namespace std;

static CommandMap cmdList;

static void FillCmdList()
{
	cmdList.clear();

	cmdList[1]="Change From";
	cmdList[2]="Change To";
	cmdList[3]="Change Date";
	cmdList[4]="Change Info";
	cmdList[5]="Change Amount";

	cmdList[11]="List Transfer Info";

	cmdList[21]="Reconcile";
	cmdList[22]="Unreconcile";
	cmdList[23]="Delete";

	cmdList[31]="Merge all transfers with the same from and to";

	cmdList[9]="Return to Main Menu";
	cmdList[0]="Return to Main Menu";
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
	}while(i>=v.size() || !v[i]);

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

static void ChangeFrom(Finances* f,Transfer* t)
{
	int type = (t->from->type==earmark) ? 1 : 2;
	Transfer* t2 = t->Copy();
	f->UnlinkTransfer(t);
	delete t->date;
	delete t;
	while(1)
	{
		if(type==1)
			t2->from = f->ReadInAccount(f->earmarks,"from",1,0);
		else
			t2->from = f->ReadInAccount(f->locations,"from",1,0);

		if(t2->from != t2->to)
			break;
		printf("From can't be the same as To.\n");
	}
	f->LinkTransfer(t2,0);
	t2->Print();
}

static void ChangeTo(Finances* f,Transfer* t)
{
	int type = (t->from->type==earmark) ? 1 : 2;
	Transfer* t2 = t->Copy();
	f->UnlinkTransfer(t);
	delete t->date;
	delete t;
	while(1)
	{
		if(type==1)
			t2->to = f->ReadInAccount(f->earmarks,"to",1,0);
		else
			t2->to = f->ReadInAccount(f->locations,"to",1,0);

		if(t2->from != t2->to)
			break;
		printf("From can't be the same as To.\n");
	}
	f->LinkTransfer(t2,0);
	t2->Print();
}

static void ChangeAmount(Finances* f,Transfer* t)
{
	Transfer* t2 = t->Copy();
	f->UnlinkTransfer(t);
	delete t->date;
	delete t;
	t2->amount = ReadInTotal();
	f->LinkTransfer(t2,0);
	t2->Print();
}

int RunCommand(Finances* f,Transfer* t,int cmd)
{
	if(cmdList[cmd]=="")
		return 1;

	switch(cmd)
	{
		case 1: ChangeFrom(f,t); return 0;
		case 2: ChangeTo(f,t); return 0;
		case 3: ChangeDate(t); return 1;
		case 4: t->info = ReadInInformation(); return 1;
		case 5: ChangeAmount(f,t); return 0;

		case 11: t->Print(); return 1;

		case 21: Reconcile(t,1); return 1;
		case 22: Reconcile(t,0); return 1;
		case 23: Delete(f,t); return 0;

		case 31: f->Merge(t); return 0;

		case 9: return 0;
		case 0: return 0;
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

	printf("hi\n");
}
