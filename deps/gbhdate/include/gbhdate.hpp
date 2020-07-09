/*************************************************
 *   G BRENT HURST
 *   gbhdate.hpp
 *   v1.0
 *
 *   20200708
 *
 *   A class that deals with dates of the
 *   modern Western Gregorian calendar.
 *
 *   This class currently doesn't support the
 *   Julian calendar and actually assumes it was
 *   never used. I might add support in the          TODO
 *   future. As of now, this doesn't support
 *   dates before January 1, AD 1 (Gregorian).       TODO
 *
 ************************************************/

#ifndef GBHDATE_H
#define GBHDATE_H

#include <string>
using namespace std;


class GBH_Date
{
  protected:
	int year;
	int month;
	int day;

	bool SetDate_Protected(int y, int m, int d);
	bool ZeroOut();

  public:
	//pretty much just for error checking
	void Print() const;

	int GetYear() const;
	int GetMonth() const;
	int GetDay() const;

	int GetDate() const;

	string GetMonthName() const;

	GBH_Date(int y,int m,int d);
	GBH_Date(int i);
	GBH_Date();

	bool SetDate(int y, int m, int d);
	bool SetDate(int i);

	void ReadInDate();

	bool operator<(const GBH_Date& d);
	bool operator>(const GBH_Date& d);
	bool operator<=(const GBH_Date& d);
	bool operator>=(const GBH_Date& d);
	bool operator==(const GBH_Date& d);
	bool operator!=(const GBH_Date& d);

	/* GBH_Date operator+(const int days); */
	/* GBH_Date operator-(const int days); */
	/* GBH_Date operator++(); */
	/* GBH_Date operator--(); */
	/* GBH_Date operator+=(const int days); */
	/* GBH_Date operator-=(const int days); */
	/* GBH_Date operator=(const GBH_Date& d); */
	/* int operator-(const GBH_Date& d); */
};

#endif
