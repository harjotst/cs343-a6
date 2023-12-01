#include <uSemaphore.h>
#include <algorithm>

using namespace std;

_Task Groupoff {
	Printer * prt;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	WATCard* WATCards;
	unsigned int add_WATCard_index;
	uLock lock;


	void main() {
		// wait until all student request a gift-card to start distributing them
		suspend();
		// then shuffle this array to ensure the gift card distribution is random
		random_shuffle(begin(WATCards), end(WATCards));

		for(int i = 0; i < numStudent; i++) {
			// attempt the except of the Groupoff destructor without the acceptor blocking
			// this causes groupoff to perform a yielding busy-wait on calls to its destructor
			_Accept(Groupoff) {
				break;

			// if no call to destructor continue distributing giftcards
			// note: _Else is not busy waiting because there are a finite number of students.
			} _Else {}

			yield(groupOffDelay);
		
			// give the ith WATCard in the array a giftcard by
			WATCards[i]->FWATCard = WATCards[i];
		}
	}

  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) : prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
		WATCards = new WATCard[numStudents];
		add_WATCard_index = 0;
	}
	
	// called by all student
	WATCard::FWATCard giftCard() {
		// no barging prevention, but doesn't matter here just need to set up an array of WATCards in arbitrary order
		lock.acquire();
		unsigned int idx = add_WATCard_idx;
		add_WATCard_index_count += 1;
		lock.release();

		WATCard* wc = new WATCard();
		wc->deposit(sodaCost);
		WATCards[idx] = wc;
		
		if(idx == numStudents) {
			resume();
		}

		return wc->FWATCard;
	}
};