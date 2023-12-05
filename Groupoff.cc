#include "Groupoff.h"
#include "Printer.h"

#include <uCobegin.h>
#include <algorithm>

void Groupoff::main()
{
    // co << "go 9" << endl;
    printer.print(Printer::Groupoff, 'S');

    for (unsigned int i = 0; i < numStudents; i++)
    {
        _Accept(giftCard);
    }

    // then shuffle this array to ensure the gift card distribution is random
    random_shuffle(begin(FWATCards), end(FWATCards));

    for (unsigned int i = 0;; i++)
    {
        // attempt the except of the Groupoff destructor without the acceptor blocking
        // this causes groupoff to perform a yielding busy-wait on calls to its destructor
        _Accept(~Groupoff)
        {
            // free memory of delivered giftcards
            for (WATCard *card : deliveredWATCards)
            {
                delete card;
            }

            // break to terminate main
            break;

            // if no call to destructor continue distributing giftcards
            // note: _Else is not busy waiting because there are a finite number of students.
        }
        _When(i < numStudents) _Else {}

        yield(groupoffDelay);

        // give the ith WATCard in the vector a giftcard
        WATCard *wc = new WATCard();
        wc->deposit(sodaCost);
        FWATCards[i].delivery(wc);
        deliveredWATCards.push_back(wc);
    }

    // co << "go 49" << endl;
    printer.print(Printer::Groupoff, 'F');
}

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {}

// called by all students
WATCard::FWATCard Groupoff::giftCard()
{
    WATCard::FWATCard fwc;

    FWATCards.push_back(fwc);

    // cout << "go 63" << endl;
    printer.print(Printer::Groupoff, 'D', sodaCost);

    return fwc;
}