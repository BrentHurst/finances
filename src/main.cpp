#include "finances.hpp"
#include <exception>

using namespace std;

int main()
{
	Finances f;

	try
	{
		f.Run();
	}
	catch(runtime_error &e)
	{
		printf("Caught a runtime error:\n\n%s\n", e.what());
		exit(1);
	}

	return 0;
}
