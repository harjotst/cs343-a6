#include "WATCardOffice.h"
#include "Printer.h"
#include "Bank.h"

#include <uPRNG.h>
#include <uFuture.h>
#include <uCobegin.h>
#include <uPRNG.h>

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers)
    : printer(prt), bank(bank), numCouriers(numCouriers), closePool(0) {}

WATCardOffice::Courier::Courier(Printer &printer, WATCardOffice &office, unsigned int id)
    : printer(printer), office(office), id(id) {};

void WATCardOffice::Courier::main()
{
    printer.print(Printer::Courier, id, 'S');

    for (;;)
    {
        Job *job = office.requestWork();

        /* nullptr signals end of jobs - triggered by WATCard
           office destruction break to terminate main */
        if (job == nullptr)
            break;

        printer.print(Printer::Courier, id, 't', job->args.sid, job->args.amount);

        /* handle job request - communication with the bank blocks
           until enough funds in students account to withdraw */
        office.bank.withdraw(job->args.sid, job->args.amount);

        if (prng(6) == 0) // check if WATCard is lost (1/6 chance)
        {
            printer.print(Printer::Courier, id, 'L', job->args.sid);

            job->result.exception(new WATCardOffice::Lost()); // signal a Lost exception
        }
        else
        {
            // deposit the funds into the WATCard
            job->args.card->deposit(job->args.amount);

            // signal the WATCard is ready by setting the future
            job->result.delivery(job->args.card);

            printer.print(Printer::Courier, id, 'T', job->args.sid, job->args.amount);
        }

        // delete the Job since it was declared on heap
        delete job;
    }

    printer.print(Printer::Courier, id, 'F');
}

/* called by student to create a new WATCard return a future WATCard */
WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount)
{
    /* create a 'real' WATCard with an inital balance
       put onthe heap so it persists after the return */
    WATCard *card = new WATCard();

    deliveredWATCards.push_back(card);

    WATCard::FWATCard fwc;

    job_buf.push_back(new Job(Args{card, fwc, amount, sid}));

    printer.print(Printer::WATCardOffice, 'C', sid, amount);

    /* return the future WATCard so the student
       can wait on its update to continue purchase */
    return fwc;
}

/* called by student when their card indicates
   insufficent funds return a future WATCard */
WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card)
{
    WATCard::FWATCard fwc;

    /* add the job to the back of the queue */
    job_buf.push_back(new Job(Args{card, fwc, amount, sid}));

    printer.print(Printer::WATCardOffice, 'T', sid, amount);

    // return the future WATCard so the student can wait on its update to continue purchase
    return fwc;
}

// blocks until a Job request is ready, and receives next request as result of call - money sent from parent
WATCardOffice::Job *WATCardOffice::requestWork()
{
    // if the pool closed flag has been set after the WATCardOffice destructor is called
    // return nullptr to signal the Courier task to terminate
    if (closePool == 1)
        return nullptr;

    // otherwise return the first job in queue (FIFO) and remove from the buffer
    Job *job = job_buf.front();

    job_buf.pop_front();

    printer.print(Printer::WATCardOffice, 'W');

    return job;
}

void WATCardOffice::main()
{
    printer.print(Printer::WATCardOffice, 'S');

    // create a courier pool of numCouriers and pass each courier task a pointer to WATCardOffice
    for (unsigned int i = 0; i < numCouriers; i++)
        couriers.push_back(new Courier(printer, *this, i));

    for (;;)
    {
        _When(!job_buf.empty()) _Accept(requestWork);
        or _Accept(create || transfer);
        or _Accept(~WATCardOffice)
        {
            // only called when all job requests have been handled -> buffer is empty
            // all numCouriers tasks will be waiting on requestWork

            closePool = 1;

            // resume each courier task waiting on requestWork
            // they will see the pool has closed and terminate main
            for (unsigned int i = 0; i < numCouriers; i++)
            {
                _Accept(requestWork);
            }

            // delete all the courier tasks now that they have finished execution
            for (unsigned int i = 0; i < numCouriers; i++)
            {
                delete couriers[i];
            }

            // free memory of delivered WATCards
            for (WATCard *card : deliveredWATCards)
            {
                delete card;
            }

            break;
        }
    }

    printer.print(Printer::WATCardOffice, 'F');
}