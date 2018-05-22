#ifndef TRACOMP_H
#define TRACOMP_H

using namespace std;


class Transaction;
class Transfer;

class tracomp
{
	public:
		bool operator() (const Transaction* a,const Transaction* b);
		bool operator() (const Transfer* a,const Transfer* b);
};

#endif
