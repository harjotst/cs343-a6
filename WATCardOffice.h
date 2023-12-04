#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H

#include <uPRNG.h>
#include <iostream>
#include <cstdlib> // access: rand, srand
#include <unistd.h>	
#include <string>			
#include <cmath>		
#include <iomanip>	
#include <fstream>
#include <utility>
#include <vector>
#include <deque>

#include <uFuture.h>
#include <uCobegin.h>
#include <uPRNG.h>	

#include "watcard.h"
#include "bank.h"
#include "Printer.h"

using namespace std;


_Task WATCardOffice {
	struct Args {
		WATCard* card;
		WATCard::FWATCard fwc;
		unsigned int amount;
		unsigned int sid;
	};

	struct Job {
		Args args;
		WATCard::FWATCard result;
		Job( Args args ) : args(args) {
			result = args.fwc;
		};
	};

	Printer * prt;
	Bank * bank;
	unsigned int numCouriers;
	bool closePool;
	deque<Job*> job_buf;
	vector<WATCard*> deliveredWATCards;

	_Task Courier {
		WATCardOffice *office;

		void main();

	  public:
		Courier(WATCardOffice * office);

	};

	vector<Courier*> couriers;


	void main();



  public:

	_Event Lost {};

	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );

	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));

	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));

	Job * requestWork() __attribute__(( warn_unused_result ));
};

#endif // WATCARDOFFICE_H
