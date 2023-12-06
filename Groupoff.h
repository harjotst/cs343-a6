#ifndef GROUPOFF_H
#define GROUPOFF_H

#include <vector>

#include "WATCard.h"

_Monitor Printer;

using namespace std;

_Task Groupoff
{
	Printer & printer;

	unsigned int numStudents, sodaCost, groupoffDelay;

	vector<WATCard::FWATCard> fWATCards;

	vector<WATCard *> deliveredWATCards;

	void main();

public:
	Groupoff(Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay);

	WATCard::FWATCard giftCard();
};

#endif // GROUPOFF_H