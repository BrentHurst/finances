#include "finances.hpp"
#include "nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <map>

using namespace std;
using nlohmann::json;

typedef std::runtime_error SRE;

void Finances::CheckHeadAccountExists(const string& name)
{
	if(AllAccounts.find(name) == AllAccounts.end())
		throw SRE(ErrorAsterisks + "  Error: The head account " + name + "doesn't exist for some reason.");
}

void Finances::CheckDeleteAccountExists(const string& name, const string& type, const string& parname)
{
	map<string,Account*>::iterator mit;
	Account* acc;

	if((mit = AllAccounts.find(name)) == AllAccounts.end())
	{
		acc = new Account;
		acc->Name = name;
		acc->Type = type;
		acc->Currency = DefaultCurrency;
		acc->Parent = AllAccounts[parname];
		acc->Parent->Children[name] = acc;
		AllAccounts[acc->Name] = acc;

		printf("For some reason %s didn't exist as an account. It has been added. Be careful not to remove it.\n",name.c_str());
	}
	else if(mit->second->Children.size())
	{
		throw SRE(ErrorAsterisks + "  Error: Somehow " + name + " has child accounts.");
	}
	else if(!mit->second->Parent)
	{
		mit->second->Parent = AllAccounts[parname];
		mit->second->Parent->Children[name] = mit->second;

		printf("For some reason %s didn't have a parent account. Its parent account should be %s. It has been corrected. Be careful not to change that.\n",name.c_str(),parname.c_str());
	}
	else if(mit->second->Parent != AllAccounts[parname])
	{
		throw SRE(ErrorAsterisks + "  Error: Somehow " + name + " has a parent account that isn't " + parname + ".");
	}
}

void Finances::CheckHeadsAndDeletes()
{
	CheckHeadAccountExists(HEAD_TAG_NAME);
	CheckHeadAccountExists(HEAD_LOCATION_NAME);
	CheckHeadAccountExists(HEAD_EARMARK_NAME);
	CheckHeadAccountExists(HEAD_TOFROM_NAME);

	CheckDeleteAccountExists(DELETE_TAG_NAME, "Tag", HEAD_TAG_NAME);
	CheckDeleteAccountExists(DELETE_LOCATION_NAME, "Location", HEAD_LOCATION_NAME);
	CheckDeleteAccountExists(DELETE_EARMARK_NAME, "Earmark", HEAD_EARMARK_NAME);
	CheckDeleteAccountExists(DELETE_TOFROM_NAME, "ToFrom", HEAD_TOFROM_NAME);
}
