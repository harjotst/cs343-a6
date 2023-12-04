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
#include "Bank.h"
#include "Printer.h"

using namespace std;


_Task WATCardOffice {
	// each job needs to know which WATCard to deposit to, amount to deposit, and which student
	struct Args {
		WATCard* card;
		WATCard::FWATCard fwc;
		unsigned int amount;
		unsigned int sid;
	};

	struct Job { // marshalled arguments and return future -> Q - whyyyy??
		Args args; // call arguments
		WATCard::FWATCard result;
		Job( Args args ) : args( args ) {
			result = args.fwc;
		}
	};

	Printer * prt;
	Bank * bank;
	unsigned int numCouriers;

	deque<Job*> job_buf;

	_Event ClosePool {};

	_Task Courier {
		WATCardOffice *office;

		void main() {
			// handle the job requests in the order they come in
			for(;;) {

				// blocks until a job request is ready
				Job *job = office->requestWork();

				if (job == nullptr) {
				 	break;
				}


				// handle job request - communication with the bank
                // blocks until enough funds in students account to withdraw
				office->bank->withdraw(job->args.sid, job->args.amount);


				// update the students WATCard
                // check if WATCard is lost (1/6 chance)
				unsigned int lost_WATCard = prng(6);
				if(lost_WATCard == 0) {
					// set the futures exception and set the fwc pointer to wc to singal its update
					WATCardOffice::Lost *exception = new WATCardOffice::Lost();
					job->result.exception(exception);
                    job->result.delivery(job->args.card);
				
				} else {
					// deposit the funds into the WATCard
                    // might block if another task is accessing the WATCard
					job->args.card->deposit(job->args.amount);

					job->result.delivery(job->args.card); // signal the WATCard is ready by setting the future
				}

				// delete the Job since it was declared on heap (ensure it isn't deleting the WATCard)
				delete job;
			}
		}

	  public:
		Courier(WATCardOffice * office) : office(office) {};

	}; // communicates with bank

	vector<Courier*> couriers;

	
  public:
  
	_Event Lost {}; // lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) : prt(&prt), bank(&bank), numCouriers(numCouriers) {
	}

	
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result )) {
		// create a 'real' WATCard with an inital balance
		// put on the heap so it persists after the return
		WATCard* card = new WATCard();

		WATCard::FWATCard fwc;

		job_buf.push_back(new Job(Args{ card, fwc, amount,  sid}));
		
		// return the future WATCard so the student can wait on its update to continue purchase
		return fwc;
	}

	// called by student when their card indicates insufficent funds
	// return a future WATCard - the student can then call select on this ??
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result )) {
		WATCard::FWATCard fwc;
        
		// add the job to the back of the queue
		job_buf.push_back(new Job(Args{ card, fwc, amount,  sid}));

		// return the future WATCard so the student can wait on its update to continue purchase
		return fwc;
	}

	// blocks until a Job request is ready, and receives next request as result of call - money sent from parent
	Job * requestWork() __attribute__(( warn_unused_result )) {
		// try to get a job from the buffer
		if(job_buf.empty()) {
			try {
				_Accept(create || transfer);

			} _Catch(uMutexFailure::RendezvousFailure &e) {
				return nullptr;
			} 
		}
		
		Job *job = job_buf.front();
		job_buf.pop_front();
		
		return job;
	}

  private:

	void main() {
		// create a courier pool of numCouriers
		
		// pass each courier task the watcard office
		for(int i = 0; i < numCouriers; i++) {
			couriers.push_back(new Courier(this));
		}

		_Accept(create || transfer || requestWork) {}
		or _Accept(~WATCardOffice) {
			cout << "start courier deletion" << endl;
			// only called when all job requests have been handled -> buffer is empty
			for(int i = 0; i < numCouriers; i++) {
				_Resume ClosePool() _At *couriers[i];
				delete couriers[i];
			}
		}
	}
};