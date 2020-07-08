#ifndef FINANCES_H
#define FINANCES_H

#include <map>
#include <string>
#include "utilities.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using nlohmann::json;


class Finances
{
	protected:
		void FillCmdList();

		void PrintCommands();
		int ReadInCommand();
		int RunCommand(int cmd);

		void LoadFromFile();
		void SaveToFile();
		void FromJson(const json& j);
		json ToJson();


		map<int,string,cmdcomp> CmdList;
		string filename;


	public:
		Finances(const string& fn);
		void Run();
};

#endif
