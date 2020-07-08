#ifndef FINANCES_H
#define FINANCES_H

#include <map>
#include <string>
#include "utilities.hpp"

using namespace std;


class Finances
{
	protected:
		void FillCmdList();

		void PrintCommands();
		int ReadInCommand();
		int RunCommand(int cmd);


		map<int,string,cmdcomp> CmdList;


	public:
		Finances(const string& filename);
		void Run();
};

#endif
