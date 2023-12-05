#include "Parent.h"
#include "Printer.h"
#include "Bank.h"

#include <uPRNG.h>

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay)
    : printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}

Parent::~Parent()
{
    printer.print(Printer::Parent, 'F');
}

void Parent::main()
{
    // cout << "par 9" << endl;
    printer.print(Printer::Parent, 'S');

    int counter = 0;

    // distribute money to students until destructor is called
    for (;;)
    {
        // attempt the except of the Parent destructor without the acceptor blocking
        // this causes parent to perform a yielding busy-wait on calls to its destructor
        _Accept(~Parent)
        {
            break;
        }
        _Else
        {
            yield(parentalDelay);

            unsigned int sid = prng(0, numStudents - 1); // prng(l, u) [l,u]

            unsigned int amount = prng(1, 3);

            printer.print(Printer::Parent, 'D', sid, amount);

            bank.deposit(sid, amount);
        }
    }
}
