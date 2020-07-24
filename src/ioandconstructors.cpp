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
	DefaultCurrency.clear();

	AllAccounts.clear();

	HeadLocation = NULL;
	HeadEarmark = NULL;
	HeadTag = NULL;
	HeadToFrom = NULL;

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
	Name.clear();
	Amount = 0;
	Type.clear();
	Currency.clear();

	Children.clear();
	Parent = NULL;

	Tras.clear();
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


	j["Macros"] = json::array();
	for(map<string, Macro*>::iterator mit = Macros.begin(); mit != Macros.end(); ++mit)
		j["Macros"].push_back(mit->second->AsJson());


	j["CurrencyConversions"] = json::array();
	for(map<string, CurrencyConversion*>::iterator mit = CurrencyConversions.begin(); mit != CurrencyConversions.end(); ++mit)
		j["CurrencyConversions"].push_back(mit->second->AsJson());


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
			Tras.push_back(new Tra);
			Tras.back()->FromJson(*jit,AllAccounts);
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



	for(mit = AllAccounts.begin(); mit != AllAccounts.end() && mit->second->Type != "Tag"; ++mit);
	if(mit != AllAccounts.end())
		for(HeadTag = mit->second; HeadTag->Parent; HeadTag = HeadTag->Parent);

	for(mit = AllAccounts.begin(); mit != AllAccounts.end() && mit->second->Type != "Location"; ++mit);
	if(mit != AllAccounts.end())
		for(HeadLocation = mit->second; HeadLocation->Parent; HeadLocation = HeadLocation->Parent);

	for(mit = AllAccounts.begin(); mit != AllAccounts.end() && mit->second->Type != "Earmark"; ++mit);
	if(mit != AllAccounts.end())
		for(HeadEarmark = mit->second; HeadEarmark->Parent; HeadEarmark = HeadEarmark->Parent);

	for(mit = AllAccounts.begin(); mit != AllAccounts.end() && mit->second->Type != "ToFrom"; ++mit);
	if(mit != AllAccounts.end())
		for(HeadToFrom = mit->second; HeadToFrom->Parent; HeadToFrom = HeadToFrom->Parent);
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
			Tras.push_back(new Tra);
			Tras.back()->FromJson(*jit, AllAccounts);
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
	j["AmountForeign"] = AmountForeign;
	j["AmountDefault"] = AmountDefault;

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
		AmountForeign = j["AmountForeign"];
	else
		FromJsonError("AmountForeign");

	if(j.find("AmountDefault") != j.end())
		AmountDefault = j["AmountDefault"];
	else
		FromJsonError("AmountDefault");
}
