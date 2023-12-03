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
	WATCard::FWATCard * FWATCards;
	unsigned int add_FWATCard_idx;
	uOwnerLock lock;
	uOwnerLock own_lock;
	uCondLock start_gc_dist;


	void main() {
		// wait until all student request a gift-card to start distributing them
		//suspend();
		cout << "try to acquire owner lock" << endl;

		own_lock.acquire();
		cout << "acquired lock from main" << endl;
		start_gc_dist.wait(own_lock);


		own_lock.release();

		// then shuffle this array to ensure the gift card distribution is random
		random_shuffle(&(FWATCards[0]), &(FWATCards[numStudents]));

		for(int i = 0; i < numStudents; i++) {
			// attempt the except of the Groupoff destructor without the acceptor blocking
			// this causes groupoff to perform a yielding busy-wait on calls to its destructor
			_Accept(~Groupoff) {
				break;

			// if no call to destructor continue distributing giftcards
			// note: _Else is not busy waiting because there are a finite number of students.
			} _Else {}

			yield(groupoffDelay);
		
			// give the ith WATCard in the array a giftcard by
			WATCard* wc = new WATCard();
			wc->deposit(sodaCost);
			FWATCards[i].delivery(wc);
		}
	}

  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) : prt(&prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
		FWATCards = new WATCard::FWATCard[numStudents];
		add_FWATCard_idx = 0;
	}
	
	// called by all student
	WATCard::FWATCard giftCard() {
		cout << "called giftcard from function" << endl;
		// no barging prevention, but doesn't matter here just need to set up an array of WATCards in arbitrary order
		cout << "try to acquire own lock" << endl;
		own_lock.acquire();		
		cout << "acquired lock" << endl;
		unsigned int idx = add_FWATCard_idx;
		add_FWATCard_idx += 1;
		lock.release();

		WATCard::FWATCard fwc; 
		FWATCards[idx] = fwc;
		
		if(idx == numStudents) {
			//resume();// maybe replace with resume?
			own_lock.acquire();
			cout << "acquired lock from giftcard to signal" << endl;
			start_gc_dist.signal();
			own_lock.release();
		}

		cout << "return" << endl;
		return fwc;
	}
};