/*************************************************
 *   G BRENT HURST
 *   print.cpp
 *   May 10, 2018 (original)
 *   May 11, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
using namespace std;

bool cmdcomp::operator() (int a,int b)
{
	if(b==a) return false;
	if(b==0) return true;
	if(a==0) return false;
	if(b==9) return true;
	if(a==9) return false;
	if(b==8) return true;
	if(a==8) return false;
	return (a<b);
}
