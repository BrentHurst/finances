#ifndef CMDCOMP_H
#define CMDCOMP_H

#include <map>
#include <string>
using namespace std;


class cmdcomp
{
	public:
		bool operator() (const int a,const int b);
};

typedef map<int,string,cmdcomp> CommandMap;

#endif
