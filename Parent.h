#ifndef PARENT_H
#define PARENT_H

#include "Printer.h"
#include "bank.h"
#include <uPRNG.h>


_Task Parent {
	Printer * prt;
	Bank * bank;
	unsigned int numStudents;
	unsigned int parentalDelay;

	void main();

  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif // PARENT_H