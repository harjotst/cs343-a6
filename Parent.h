#include "Printer.h"
#include <uPRNG.h>


_Task Parent {
	Printer * prt;
	Bank * bank;
	unsigned int numStudents;
	unsigned int parentalDelay;

		
	void main() {
		// distribute money to students until destructor is called
		for(;;) {
			// attempt the except of the Parent destructor without the acceptor blocking
			// this causes parent to perform a yielding busy-wait on calls to its destructor
			_Accept(~Parent) {
				break;

			// if no call to destructor continue distributing money
			} _Else {}

			yield(parentalDelay);
			unsigned int sid = prng(0, numStudents - 1); // prng(l, u) [l,u]
			unsigned int amount = prng(1, 3);

			bank->deposit(sid, amount);
		}
	}

  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) : prt(&prt), bank(&bank), numStudents(numStudents), parentalDelay(parentalDelay) {}
};