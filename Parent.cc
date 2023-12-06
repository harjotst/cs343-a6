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
    printer.print(Printer::Parent, 'S');

    int counter = 0;

    for (;;)
    {
        /* Distribute money to random student until destrutor is called. */
        _Accept(~Parent)
        {
            break;
        }
        _Else
        {
            yield(parentalDelay);

            unsigned int sid = prng(0, numStudents - 1);

            unsigned int amount = prng(1, 3);

            printer.print(Printer::Parent, 'D', sid, amount);

            bank.deposit(sid, amount);
        }
    }
}
