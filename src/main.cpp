#include "finances.hpp"
#include <exception>
#include "utilities.hpp"

using namespace std;

void usage(char* argv0)
{
	fprintf(stderr,"usage: %s inputfile\n",argv0);
}

int main(int argc, char** argv)
{
	string filename;
	Finances* f;
	int newfin;

	if(argc != 2)
	{
		usage(argv[0]);
		if(!AskSetUpNewFinances())
			exit(2);

		filename = ReadInFilename();

		if(!filename.size())
			exit(22);

		newfin = 1;
	}
	else
	{
		filename = argv[1];
		newfin = 0;
	}


	try
	{
		f = new Finances(filename);
		f->Run(newfin);
	}
	catch(runtime_error &e)
	{
		fprintf(stderr,"Caught a runtime error:\n\n%s\n", e.what());
		exit(1);
	}

	return 0;
}
