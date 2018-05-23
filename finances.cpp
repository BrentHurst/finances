#include "finances.h"
using namespace std;

Finances::Finances()
{
	locations.clear();
	earmarks.clear();
	tags.clear();
	tofroms.clear();
	transactions.clear();
	unreconciledtransactions.clear();
	transfers.clear();
	unreconciledtransfers.clear();
	conversions.clear();
	macronames.clear();
	macrotransactions.clear();
	macrotransfers.clear();
	amount=0;
}
