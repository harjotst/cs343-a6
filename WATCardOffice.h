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

	int closePool;

	deque<Job*> job_buf;

	_Task Courier {
		WATCardOffice *office;

		void main() {
			for(;;) {
				Job *job = office->requestWork();

				// nullptr signals end of jobs - triggered by WATCard office destruction
				// break to terminate main
				if(job == nullptr) {
				 	break;
				}

				// handle job request - communication with the bank
                // blocks until enough funds in students account to withdraw
				office->bank->withdraw(job->args.sid, job->args.amount);


				// update the students WATCard:
                
				// check if WATCard is lost (1/6 chance)
				unsigned int lost_WATCard = prng(6);
				if(lost_WATCard == 0) {
					// signal a Lost exception 
					job->result.exception(new WATCardOffice::Lost());

					// delete the WATCard since the student cannot
					delete job->args.card;

				} else {
					// deposit the funds into the WATCard
					job->args.card->deposit(job->args.amount);

					// signal the WATCard is ready by setting the future
					job->result.delivery(job->args.card); 
				}

				// delete the Job since it was declared on heap
				delete job;
			}
		}

	  public:
		Courier(WATCardOffice * office) : office(office) {};

	};

	vector<Courier*> couriers;

	
  public:
  
	_Event Lost {}; // lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) : prt(&prt), bank(&bank), numCouriers(numCouriers), closePool(0) {}

	// called by student to create a new WATCard
	// return a future WATCard	
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
	// return a future WATCard
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result )) {
		WATCard::FWATCard fwc;
        
		// add the job to the back of the queue
		job_buf.push_back(new Job(Args{ card, fwc, amount,  sid}));

		// return the future WATCard so the student can wait on its update to continue purchase
		return fwc;
	}

	// blocks until a Job request is ready, and receives next request as result of call - money sent from parent
	Job * requestWork() __attribute__(( warn_unused_result )) {
		// if the pool closed flag has been set after the WATCardOffice destructor is called
		// return nullptr to signal the Courier task to terminate
		if(closePool == 1) {
			return nullptr;
		}
		
		// otherwise return the first job in queue (FIFO) and remove from the buffer
		Job *job = job_buf.front();
		job_buf.pop_front();
		
		return job;
	}

  private:

	void main() {
		// create a courier pool of numCouriers and pass each courier task a pointer to WATCardOffice
		for(int i = 0; i < numCouriers; i++) {
			couriers.push_back(new Courier(this));
		}

		for(;;) {
			_When(!job_buf.empty()) _Accept(requestWork) {}
			or _Accept(create || transfer) {}
			or _Accept(~WATCardOffice) {
				// only called when all job requests have been handled -> buffer is empty
				// all numCouriers tasks will be waiting on requestWork

				closePool = 1;

				// resume each courier task waiting on requestWork
				// they will see the pool has closed and terminate main
				for(int i = 0; i < numCouriers; i++) {
					_Accept(requestWork);
				}

				// delete all the courier tasks now that they have finished execution
				for(int i = 0; i < numCouriers; i++) {
					delete couriers[i];
				}

				break;
			}
		}
	}
};