#include <uPRNG.h>
#include <uSemaphore.h>
#include <iostream>
#include <cstdlib> // access: rand, srand
#include <unistd.h>	
#include <string>			
#include <cmath>		
#include <iomanip>	
#include <fstream>

_Task WATCardOffice {
	Printer * prt;
	Bank * bank;
	unsigned int numCouriers;

	// members for buffer
	unsigned int front, back;
	uSemaphore full, empty;
	unsigned int buf_size;
	Job* job_buf;

    // each job needs to know which WATCard to deposit to, amount to deposit, and which student
	struct Args {
		WATCard* wc;
		unsigned int amount;
		unsigned int sid;
	}

	struct Job { // marshalled arguments and return future -> Q - whyyyy??
		Args args; // call arguments
		WATCard::FWATCard result; // return future ???
		Job( Args args ) : args( args ) {}
	};

	_Task Courier {
		void main() {
			// handle the job requests in the order they come in
			for(;;) {
				// blocks until a job request is ready
				Job *job = requestWork();

				// handle job request - communication with the bank
                // blocks until enough funds in students account to withdraw
				bank->withdraw(job->sid, job->amount);

				// update the students WATCard
                // check if WATCard is lost (1/6 chance)
				unsigned int lost_WATCard = prng(6);
				if(lost_WATCard == 0) {
					// need this exception to persist => Q - put on heap?
					WATCardOffice::Lost lost = new WATCardOffice::Lost();
					// update fwc pointer to point at lost exception - Q - this pointer setting seems off
                    job->wc->FWATCard = &lost;
				
				} else {
					// deposit the funds into the WATCard
                    // might block if another task is accessing the WATCard
                    // only happens on transfer calls? - since not a new WATCard
					wc->deposit(job->amount);

					job->wc->FWATCard = &wc; // signal the WATCard is ready by setting pointer to itself
				}

				// delete the Job since it was declared on heap (ensure it isn't deleting the WATCard)
				delete job;
			}
		}


	}; // communicates with bank


	void main() {
		// create a courier pool of numCouriers
		{
			Courier couriers[numCouriers];
		}
		// just wait for them to finish then terminate
	}

  public:
	_Event Lost {}; // lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) : prt(ptr), bank(bank), numCouriers(numCouriers), empty(numStudents), full(0) {
		buf_size = 10;
		job_buf = new Job[buf_size];
	}

	
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result )) {
		// create a 'real' WATCard with an inital balance
		// put on the heap so it persists after the return
		WATCard* wc = new WATCard();

		// use a courier to satisfy this create request, by adding it to the job queue
		empty.P(); // wait if the jobs buffer is full

		// add the job to the back of the queue
		job_buf[back] = new Args{ wc, amount,  sid};
		back = ( back + 1 ) % buf_size;

		full.V(); // signal a new job added to buffer

		// return the future WATCard so the student can wait on its update to continue purchase
		return wc->FWATCard;
	}

	// called by student when their card indicates insufficent funds
	// return a future WATCard - the student can then call select on this ??
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result )) {
        // use a courier to satisfy this request, by adding it to the job queue
		empty.P(); // wait if the jobs buffer is full

		// add the job to the back of the queue
		job_buf[back] = new Args{ wc, amount,  sid};
		back = ( back + 1 ) % buf_size;

		full.V(); // signal a new job added to buffer

		// return the future WATCard so the student can wait on its update to continue purchase
		return card->FWATCard;
	}

	// blocks until a Job request is ready, and receives next request as result of call - money sent from parent
	Job * requestWork() __attribute__(( warn_unused_result )) {
		// try to get a job from the buffer
		full.P(); // wait if buffer is empty

		// get the job first in queue (FIFO)
		Args *job = job_buf[front];

		// update the pointer to next job
		front = (front + 1) % buf_size;

		empty.V(); // signal empty buffer slot

		return job;
	}
};