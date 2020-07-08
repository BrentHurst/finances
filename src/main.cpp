#include "finances.hpp"
#include <exception>

using namespace std;

int main()
{
	Finances f;

	f.Run();

	/* try
	{
		f.Run();
	}
	catch(runtime_error &e)
	{
		fprintf(stderr,"Caught a runtime error:\n\n%s\n", e.what());
		exit(1);
	} */

	return 0;
}
