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
	/* FillCmdList(); */

	Clear();

	filename = fn;
	LoadFromFile();
}

void Finances::Clear()
{
	Locations.clear();
	Earmarks.clear();
	Tags.clear();
	ToFroms.clear();
	AllAccounts.clear();

	HeadLocation = NULL;
	HeadEarmark = NULL;
	HeadTag = NULL;
	HeadToFrom = NULL;
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
	ofstream ofs;

	ofs.clear();
	ofs.open(filename, ofstream::out);
	if(!ofs.is_open())
		throw SRE("Finances::SaveToFile(): Couldn't open file " + filename);

	ofs << AsJson().dump(4) << endl;
}





Account::Account()
{
	Amount = 0;
	Name.clear();
	Type.clear();

	Children.clear();
	Parent = NULL;

	Tras.clear();
}

json Account::AsJson()
{
	json j;

	j = json::object();

	j["Name"] = Name;
	j["Amount"] = Amount;
	j["Type"] = Type;
	j["Currency"] = Currency;

	return j;
}

void Account::FromJsonError(const string& s)
{
	fprintf(stderr,"%s\n\tError reading Account JSON: No %s key.\n",ErrorAsterisks.c_str(),s.c_str());
}

void Account::FromJson(const json& j)
{
	if(!j.is_object())
	{
		fprintf(stderr,"%s\n\tAccount JSON isn't an object.\n",ErrorAsterisks.c_str());
		return;
	}

	if(j.find("Name") != j.end())
		Name = j["Name"];
	else
		FromJsonError("Name");

	if(j.find("Amount") != j.end())
		Amount = j["Amount"];
	else
		FromJsonError("Amount");

	if(j.find("Type") != j.end())
		Type = j["Type"];
	else
		FromJsonError("Type");

	if(j.find("Currency") != j.end())
		Currency = j["Currency"];
	else
		FromJsonError("Currency");
}


json Tra::AsJson()
{
	json j;

	j = json::object();

	j["Type"] = Type;
	j["Id"] = Id;
	j["Date"] = Date;
	j["Info"] = Info;
	j["Reconciled"] = Reconciled;
	j["Currency"] = Currency;
	j["Amount"] = Amount;
	j["DefaultCurrencyAmount"] = DefaultCurrencyAmount;

	if(Type == "transaction")
	{
		j["Tag"] = Tag->Name;
		j["Location"] = Location->Name;
		j["Earmark"] = Earmark->Name;
		j["ToFrom"] = ToFrom->Name;
	}
	else
	{
		j["From"] = From->Name;
		j["To"] = To->Name;
	}

	return j;
}

void Tra::FromJsonError(const string& s)
{
	fprintf(stderr,"%s\n\tError reading Tra JSON: No %s key.\n",ErrorAsterisks.c_str(),s.c_str());
}

void Tra::FromJson(const json& j, map<string, Account*>& AllAccounts)
{
	if(j.find("Type") != j.end())
		Type = j["Type"];
	else
		FromJsonError("Type");

	if(j.find("Id") != j.end())
		Id = j["Id"];
	else
		FromJsonError("Id");

	if(j.find("Date") != j.end())
		Date = j["Date"];
	else
		FromJsonError("Date");

	if(j.find("Info") != j.end())
		Info = j["Info"];
	else
		FromJsonError("Info");

	if(j.find("Reconciled") != j.end())
		Reconciled = j["Reconciled"];
	else
		FromJsonError("Reconciled");

	if(j.find("Currency") != j.end())
		Currency = j["Currency"];
	else
		FromJsonError("Currency");

	if(j.find("Amount") != j.end())
		Amount = j["Amount"];
	else
		FromJsonError("Amount");

	if(j.find("DefaultCurrencyAmount") != j.end())
		DefaultCurrencyAmount = j["DefaultCurrencyAmount"];
	else
		FromJsonError("DefaultCurrencyAmount");

	if(Type == "transaction")
	{
		if(j.find("Tag") != j.end())
			Tag = AllAccounts[j["Tag"]];
		else
			FromJsonError("Tag");

		if(j.find("Location") != j.end())
			Location = AllAccounts[j["Location"]];
		else
			FromJsonError("Location");

		if(j.find("Earmark") != j.end())
			Earmark = AllAccounts[j["Earmark"]];
		else
			FromJsonError("Earmark");

		if(j.find("ToFrom") != j.end())
			ToFrom = AllAccounts[j["ToFrom"]];
		else
			FromJsonError("ToFrom");
	}
	else
	{
		if(j.find("From") != j.end())
			From = AllAccounts[j["From"]];
		else
			FromJsonError("From");

		if(j.find("To") != j.end())
			To = AllAccounts[j["To"]];
		else
			FromJsonError("To");
	}
}



json Finances::AsJson()
{
	json j;
	json j1;

	j = json::object();

	j["DefaultCurrency"] = DefaultCurrency;

	j["Accounts"] = json::array();
	for(map<string, Account*>::iterator mit = AllAccounts.begin(); mit != AllAccounts.end(); ++mit)
		j["Accounts"].push_back(mit->second->AsJson());

	j["ParentAccountInfo"] = json::array();
	for(map<string, Account*>::iterator mit = AllAccounts.begin(); mit != AllAccounts.end(); ++mit)
	{
		if(mit->second->Parent)
		{
			j1 = json::object();
			j1["Child"] = mit->second->Name;
			j1["Parent"] = mit->second->Parent->Name;
			j["ParentAccountInfo"].push_back(j1);
		}
	}


	j["Tras"] = json::array();
	for(unsigned int i = 0; i < Tras.size(); ++i)
		j["Tras"].push_back(Tras[i]->AsJson());

	/* j["Macros"] = ; // TODO */

	/* j["CurrencyConversions"] = ; // TODO */

	return j;
}

void Finances::FromJson(const json& j)
{
	// TODO
	// HERE
}
