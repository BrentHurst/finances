#include "finances.hpp"
#include "utilities.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using nlohmann::json;

typedef std::runtime_error SRE;


Finances::Finances(const string& fn)
{
	Clear();

	filename = fn;
}

void Finances::Clear()
{
	DefaultCurrency.clear();

	AllAccounts.clear();

	HeadTag = NULL;
	HeadLocation = NULL;
	HeadEarmark = NULL;
	HeadToFrom = NULL;

	DeleteTag = NULL;
	DeleteLocation = NULL;
	DeleteEarmark = NULL;
	DeleteToFrom = NULL;

	Macros.clear();
	Tras.clear();
	CurrencyConversions.clear();
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



void Finances::SaveToFile(int StartNewTimePeriodFromHere)
{
	ofstream ofs;

	ofs.clear();
	ofs.open(filename, ofstream::out);
	if(!ofs.is_open())
		throw SRE("Finances::SaveToFile(): Couldn't open file " + filename);

	if(StartNewTimePeriodFromHere)
	{
		// TODO
	}
	else
		ofs << AsJson().dump(4) << endl;

	ofs.close();
}

Account::Account()
{
	Name.clear();
	Amount = 0;
	Type.clear();
	Currency.clear();

	Children.clear();
	Parent = NULL;
}
Account::Account(string n, double a, string t, string c)
{
	Name = n;
	Amount = a;
	Type = t;
	Currency = c;

	Children.clear();
	Parent = NULL;
}

CurrencyConversion::CurrencyConversion()
{
	ForeignCurrency.clear();
	AmountForeign = 0;
	AmountDefault = 0;
}

Tra::Tra()
{
	Type.clear();
	Id = 0;
	Date = 0;
	Info.clear();
	Reconciled = 0;
	Currency.clear();
	Amount = 0;
	DefaultCurrencyAmount = 0;

	Tag = NULL;
	Location = NULL;
	Earmark = NULL;
	ToFrom = NULL;

	From = NULL;
	To = NULL;
}

Macro::Macro()
{
	Name.clear();
	Tras.clear();
}

json Account::AsJson()
{
	json j;

	j = json::object();

	j["Name"] = Name;
	j["Amount"] = Round2Decimals(Amount);
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
		Amount = Round2Decimals(j["Amount"]);
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
	j["Amount"] = Round2Decimals(Amount);
	j["DefaultCurrencyAmount"] = Round2Decimals(DefaultCurrencyAmount);

	if(Type == "Transaction")
	{
		j["Tag"] = Tag->Name;
		j["Location"] = Location->Name;
		j["Earmark"] = Earmark->Name;
		j["ToFrom"] = ToFrom->Name;
	}
	else if(Type == "Transfer")
	{
		j["From"] = From->Name;
		j["To"] = To->Name;
	}
	else
	{
		fprintf(stderr,"%s\t\nError: Tra whose Type isn't Transfer or Transaction. Type = %s\n",ErrorAsterisks.c_str(),Type.c_str());
	}

	return j;
}

void Tra::FromJsonError(const string& s)
{
	fprintf(stderr,"%s\n\tError reading Tra JSON: No %s key.\n",ErrorAsterisks.c_str(),s.c_str());
}

void Tra::FromJson(const json& j, map<string, Account*>& AllAccounts)
{
	if(!j.is_object())
	{
		fprintf(stderr,"%s\n\tTra JSON isn't an object.\n",ErrorAsterisks.c_str());
		return;
	}

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
		Amount = Round2Decimals(j["Amount"]);
	else
		FromJsonError("Amount");

	if(j.find("DefaultCurrencyAmount") != j.end())
		DefaultCurrencyAmount = Round2Decimals(j["DefaultCurrencyAmount"]);
	else
		FromJsonError("DefaultCurrencyAmount");

	if(Type == "Transaction")
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
	else if(Type == "Transfer")
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
	else
	{
		fprintf(stderr,"%s\t\nError: Tra whose Type isn't Transfer or Transaction. Type = %s\n",ErrorAsterisks.c_str(),Type.c_str());
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
	for(map<unsigned long long, Tra*>::iterator mit = Tras.begin(); mit != Tras.end(); ++mit)
		j["Tras"].push_back(mit->second->AsJson());


	j["Macros"] = json::array();
	for(map<string, Macro*>::iterator mit = Macros.begin(); mit != Macros.end(); ++mit)
		j["Macros"].push_back(mit->second->AsJson());


	j["CurrencyConversions"] = json::array();
	for(map<string, CurrencyConversion*>::iterator mit = CurrencyConversions.begin(); mit != CurrencyConversions.end(); ++mit)
		j["CurrencyConversions"].push_back(mit->second->AsJson());

	j["Flags"] = json::object();
	for(map<string, int>::iterator mit = Flags.begin(); mit != Flags.end(); ++mit)
		j["Flags"][mit->first] = mit->second;

	return j;
}

void Finances::FromJsonError(const string& s)
{
	fprintf(stderr,"%s\n\tError reading Macro JSON: No %s key.\n",ErrorAsterisks.c_str(),s.c_str());
}

void Finances::FromJson(const json& j)
{
	json::const_iterator jit;
	json j1;
	map<string,Account*>::iterator mit;
	Account* acc;
	Account* parent;
	Account* child;
	Tra* tra;
	Macro* mac;
	CurrencyConversion* cc;

	if(!j.is_object())
	{
		fprintf(stderr,"%s\n\tFinances JSON isn't an object.\n",ErrorAsterisks.c_str());
		return;
	}

	if(j.find("DefaultCurrency") != j.end())
		DefaultCurrency = j["DefaultCurrency"];
	else
		FromJsonError("DefaultCurrency");


	if(j.find("Accounts") != j.end())
	{
		for(jit = j["Accounts"].begin(); jit != j["Accounts"].end(); ++jit)
		{
			acc = new Account;
			acc->FromJson(*jit);
			if(AllAccounts.find(acc->Name) != AllAccounts.end())
			{
				fprintf(stderr,"%s\n\tRepeated Account Name: %s\n",ErrorAsterisks.c_str(),acc->Name.c_str());
				return;
			}
			AllAccounts[acc->Name] = acc;
		}
	}
	else
		FromJsonError("Accounts");


	if(j.find("ParentAccountInfo") != j.end())
	{
		for(jit = j["ParentAccountInfo"].begin(); jit != j["ParentAccountInfo"].end(); ++jit)
		{
			if(!(*jit).is_object())
			{
				fprintf(stderr,"%s\n\tParent Account Info item isn't object.\n",ErrorAsterisks.c_str());
			}
			else
			{
				j1 = *jit;
				if(j1.find("Parent") == j1.end() || j1.find("Child") == j1.end())
				{
					fprintf(stderr,"%s\n\tParent Account Info object isn't of the form { \"Parent\" : \"Name\" , \"Child\" : \"Name\" }\n",ErrorAsterisks.c_str());
				}
				else if(AllAccounts.find(j1["Parent"]) == AllAccounts.end() || AllAccounts.find(j1["Child"]) == AllAccounts.end())
				{
					fprintf(stderr,"%s\n\tParent Account Info object has parent or child that doesn't exist as an account.\n",ErrorAsterisks.c_str());
				}
				else
				{
					parent = AllAccounts[j1["Parent"]];
					child = AllAccounts[j1["Child"]];

					if(parent->Type != child->Type)
					{
						fprintf(stderr,"%s\n\tParent Account Info object has parent and child of differnet type.\n",ErrorAsterisks.c_str());
					}
					else if(child->Parent)
					{
						fprintf(stderr,"%s\n\t%s has multiple parents.\n",ErrorAsterisks.c_str(),child->Name.c_str());
					}
					else
					{
						child->Parent = parent;
						parent->Children[child->Name] = child;
					}
				}
			}
		}
	}
	else
		FromJsonError("ParentAccountInfo");


	if(j.find("Tras") != j.end())
	{
		for(jit = j["Tras"].begin(); jit != j["Tras"].end(); ++jit)
		{
			tra = new Tra;
			tra->FromJson(*jit,AllAccounts);
			InsertTraIntoMap(tra,Tras);
		}
	}
	else
		FromJsonError("Tras");


	if(j.find("Macros") != j.end())
	{
		for(jit = j["Macros"].begin(); jit != j["Macros"].end(); ++jit)
		{
			mac = new Macro;
			mac->FromJson(*jit,AllAccounts);

			if(Macros.find(mac->Name) != Macros.end())
			{
				fprintf(stderr,"%s\n\tRepeated Macro Name: %s\n",ErrorAsterisks.c_str(),mac->Name.c_str());
				return;
			}
			Macros[mac->Name] = mac;
		}
	}
	else
		FromJsonError("Macros");


	if(j.find("CurrencyConversions") != j.end())
	{
		for(jit = j["CurrencyConversions"].begin(); jit != j["CurrencyConversions"].end(); ++jit)
		{
			cc = new CurrencyConversion;
			cc->FromJson(*jit);

			if(CurrencyConversions.find(cc->ForeignCurrency) != CurrencyConversions.end())
			{
				fprintf(stderr,"%s\n\tRepeated Currency Conversion: %s\n",ErrorAsterisks.c_str(),cc->ForeignCurrency.c_str());
				return;
			}
			CurrencyConversions[cc->ForeignCurrency] = cc;
		}
	}
	else
		FromJsonError("CurrencyConversions");


	if(j.find("Flags") != j.end())
	{
		for(jit = j["Flags"].begin(); jit != j["Flags"].end(); ++jit)
			Flags[jit.key()] = jit.value();
	}
	SetDefaultFlagsIfFlagDoesntExist();


	CheckHeadsAndDeletes();

	SetHeadsAndDeletes();
}

void Finances::SetDefaultFlagsIfFlagDoesntExist()
{
	if(Flags.find("AskCurrency") == Flags.end())
		Flags["AskCurrency"] = 0;
	if(Flags.find("ListAccountsForNewTrasByDefault") == Flags.end())
		Flags["ListAccountsForNewTrasByDefault"] = 0;
}

void Finances::SetHeadsAndDeletes()
{
	HeadTag = AllAccounts[HEAD_TAG_NAME];
	HeadLocation = AllAccounts[HEAD_LOCATION_NAME];
	HeadEarmark = AllAccounts[HEAD_EARMARK_NAME];
	HeadToFrom = AllAccounts[HEAD_TOFROM_NAME];

	DeleteTag = AllAccounts[DELETE_TAG_NAME];
	DeleteLocation = AllAccounts[DELETE_LOCATION_NAME];
	DeleteEarmark = AllAccounts[DELETE_EARMARK_NAME];
	DeleteToFrom = AllAccounts[DELETE_TOFROM_NAME];
}


json Macro::AsJson()
{
	json j;

	j = json::object();

	j["Name"] = Name;

	j["Tras"] = json::array();
	for(unsigned int i = 0; i < Tras.size(); ++i)
		j["Tras"].push_back(Tras[i]->AsJson());

	return j;
}

void Macro::FromJsonError(const string& s)
{
	fprintf(stderr,"%s\n\tError reading Macro JSON: No %s key.\n",ErrorAsterisks.c_str(),s.c_str());
}

void Macro::FromJson(const json& j, map<string, Account*>& AllAccounts)
{
	Tra* tra;

	if(!j.is_object())
	{
		fprintf(stderr,"%s\n\tMacro JSON isn't an object.\n",ErrorAsterisks.c_str());
		return;
	}

	if(j.find("Name") != j.end())
		Name = j["Name"];
	else
		FromJsonError("Name");

	if(j.find("Tras") != j.end())
	{
		for(json::const_iterator jit = j["Tras"].begin(); jit != j["Tras"].end(); ++jit)
		{
			tra = new Tra;
			tra->FromJson(*jit, AllAccounts);
			InsertTraIntoMap(tra,Tras);
		}
	}
	else
		FromJsonError("Tras");
}


json CurrencyConversion::AsJson()
{
	json j;

	j = json::object();

	j["ForeignCurrency"] = ForeignCurrency;
	j["AmountForeign"] = Round2Decimals(AmountForeign);
	j["AmountDefault"] = Round2Decimals(AmountDefault);

	return j;
}

void CurrencyConversion::FromJsonError(const string& s)
{
	fprintf(stderr,"%s\n\tError reading Currency Conversion JSON: No %s key.\n",ErrorAsterisks.c_str(),s.c_str());
}

void CurrencyConversion::FromJson(const json& j)
{
	if(!j.is_object())
	{
		fprintf(stderr,"%s\n\tCurrency Conversion JSON isn't an object.\n",ErrorAsterisks.c_str());
		return;
	}

	if(j.find("ForeignCurrency") != j.end())
		ForeignCurrency = j["ForeignCurrency"];
	else
		FromJsonError("ForeignCurrency");

	if(j.find("AmountForeign") != j.end())
		AmountForeign = Round2Decimals(j["AmountForeign"]);
	else
		FromJsonError("AmountForeign");

	if(j.find("AmountDefault") != j.end())
		AmountDefault = Round2Decimals(j["AmountDefault"]);
	else
		FromJsonError("AmountDefault");
}
