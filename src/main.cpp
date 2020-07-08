#include "finances.hpp"
#include <exception>

using namespace std;

void usage(char* argv0)
{
	fprintf(stderr,"usage: %s inputfile\n",argv0);
}

int main(int argc, char** argv)
{
	string filename;

	if(argc != 2)
	{
		usage(argv[0]);
		exit(2);
	}

	filename = argv[1];

	try
	{
		Finances f(filename);
		f.Run();
	}
	catch(runtime_error &e)
	{
		fprintf(stderr,"Caught a runtime error:\n\n%s\n", e.what());
		exit(1);
	}

	return 0;
}
