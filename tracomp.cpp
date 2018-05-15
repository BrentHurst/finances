/*************************************************
 *   G BRENT HURST
 *   tracomp.cpp
 *   May 15, 2018 (original)
 *   May 15, 2018 (last edit)
 *
 *   #include "finances.h"
 *
 ************************************************/

#include "finances.h"
using namespace std;

template<class T>
bool tracomp<T>::operator() (T const& a,T const& b)
{
	return (*(a->date) < *(b->date));
}
