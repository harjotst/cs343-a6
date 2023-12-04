#ifndef GROUPOFF_H
#define GROUPOFF_H

#include <uCobegin.h>
#include <algorithm>
#include "watcard.h"
#include "Printer.h"

using namespace std;

_Task Groupoff {
	Printer * prt;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	vector<WATCard::FWATCard> FWATCards;
	vector<WATCard*> deliveredWATCards;

	void main();

  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	
	WATCard::FWATCard giftCard();
};

#endif // GROUPOFF_H