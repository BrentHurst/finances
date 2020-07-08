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

		int ReadInCommand();
		void PrintCommands();


		map<int,string,cmdcomp> CmdList;


	public:
		Finances();
		void Run();
};

#endif
