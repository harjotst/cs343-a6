#include <uSemaphore.h>

_Task Student {
	Printer * prt;
	NameServer * nameServer;
	WATCardOffice * cardOffice;
	Groupoff * groupoff;
	unsigned int id;
	unsigned int maxPurchases;

	void main() {
		WATCard::FWATCard fgc = Groupoff->giftCard();
		// Selects num bottles to purchase - range [1, MaxPurchases]
		// Selects favorite flavor [0, 3]


		do {
			// Creates a WAT card from WATCard office with balance of $5
			WATCard::FWATCard fwc = cardOffice->create(id, 5);

			_Select(fgc)
				// handle gift card
			or _Select(fwc)
				// handle WATCard
		} while(fwc == WATCardOffice::Lost)
	}

  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};

class WATCard {
	WATCard( const WATCard & ) = delete;	// prevent copying
	WATCard( const WATCard && ) = delete;
	WATCard & operator=( const WATCard & ) = delete;
	WATCard & operator=( const WATCard && ) = delete;
  public:
	typedef Future_ISM<WATCard *> FWATCard;	// future watcard pointer
	WATCard();
	void deposit( unsigned int amount );
	void withdraw( unsigned int amount );
	unsigned int getBalance();
};



// manages all the students account info
_Monitor Bank {
	unsigned int *student_accounts;
	uCondition *account_locks;

  public:
	Bank( unsigned int numStudents ) {
		student_accounts = new int[numStudents]; // maybe use uNoCtor - apparently C++ guarentees mem is 0 initialized does uC++?
		account_locks = new uCondition[numStudents];

		// all students start with $0
		for(int i = 0; i < numStudent; i++) {
			student_accounts[i] = 0;
		}
	}

	// destructor called by main at the end of execution
	~Bank() {
		delete [] student_accounts;
		delete [] account_locks;
	}

	void deposit( unsigned int id, unsigned int amount ) {
		student_accounts[id] += amount;
		account_locks[id].signal();
	}

	void withdraw( unsigned int id, unsigned int amount ) {
		// if the students account doesn't have enough money to withdraw amount, wait until it does
		// this can take more than one deposit
		while(student_accounts[id] < amount) {
			// wait on that specific deposit into this students account
			account_locks[id].wait();
		}

		student_accounts[id] -= amount;
	}

};

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

			bank->deposit(id, amount);
		}
	}

  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) : prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}
};

_Task Groupoff {
	Printer * prt;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	WATCard* WATCards;
	unsigned int add_WATCard_index_count;
	uLock lock;


	void main() {
		// wait until all student request a gift-card to start distributing them
		for(;;) {
			// attempt the except of the Groupoff destructor without the acceptor blocking
			// this causes groupoff to perform a yielding busy-wait on calls to its destructor
			_Accept(~groupoff) {
				break;

			// if no call to destructor continue distributing giftcards
			// note: _Else is not busy waiting because there are a finite number of students.
			} _Else {}



		}
	}

  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) : prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
		WATCards = new WATCard[numStudents];
		add_WATCard_index_count = 0;
	}
	
	// called by all student
	WATCard::FWATCard giftCard() {
		// no barging prevention, but doesn't matter here just need to set up an array of WATCards in arbitrary order
		lock.acquire();
		unsigned int IDX = add_WATCard_idx;
		add_WATCard_index_count += 1;
		lock.release();

		WATCard* wc = new WATCard();
		wc->deposit(sodaCost);
		WATCards[add_WATCard_idx] = wc;
		
		return wc->
	}
};

_Task VendingMachine {
	void main();
  public:
	_Event Free {};						// free, advertisement
	_Event Funds {};					// insufficient funds
	_Event Stock {};					// flavour out of stock
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
	void buy( BottlingPlant::Flavours flavour, WATCard & card );
	unsigned int * inventory() __attribute__(( warn_unused_result ));
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};

_Task NameServer {
	void main();
  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
	void VMregister( VendingMachine * vendingmachine );
	VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
	VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};

_Task BottlingPlant {
	void main();
  public:
	enum Flavours { ..., NUM_OF_FLAVOURS };	// flavours of soda (YOU DEFINE)
	_Event Shutdown {};					// shutdown plant
	BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	void getShipment( unsigned int cargo[] );
};

_Task Truck {
	void main();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

_Monitor / _Cormonitor Printer {
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};