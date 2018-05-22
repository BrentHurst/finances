#include "cmd.h"
#include "readtype.h"
using namespace std;


void PrintCommands(CommandMap& cmdList)
{
	CommandMap::iterator mit;

	printf("\n");
	for(mit=cmdList.begin(); mit != cmdList.end(); mit++)
		if(mit->second != "")
			printf("%2d. %s\n",mit->first,mit->second.c_str());
}

int GetCommand(CommandMap& cmdList)
{
	int i;

	PrintCommands(cmdList);

	do
	{
		printf("Please choose a command: ");
		i = ReadInt();
	}while(cmdList.find(i)==cmdList.end());

	printf("\n");

	return i;
}
