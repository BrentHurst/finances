/*************************************************
 *   G BRENT HURST
 *   tracomp.cpp
 *   May 15, 2018 (original)
 *   May 15, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "tracomp.h"
#include "transaction.h"
#include "transfer.h"
using namespace std;

bool tracomp::operator() (const Transaction* a,const Transaction* b)
{
	return (*(a->date) < *(b->date));
}

bool tracomp::operator() (const Transfer* a,const Transfer* b)
{
	return (*(a->date) < *(b->date));
}
