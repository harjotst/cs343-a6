#include "Groupoff.h"
#include "Printer.h"

#include <uCobegin.h>
#include <algorithm>

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {}

void Groupoff::main()
{
    printer.print(Printer::Groupoff, 'S');

    for (unsigned int timesAccepted = 0; timesAccepted < numStudents; timesAccepted += 1)
    {
        _Accept(giftCard);
    }

    /* Randomize order of future watcards as to facilitate random
       deliveries of real WATCards. */
    random_shuffle(begin(fWATCards), end(fWATCards));

    for (unsigned int cardDelivered = 0;; cardDelivered += 1)
    {
        /* Attempt accepting the destructor without the acceptor blocking.
           This causes Groupoff to perform a yielding busy-wait on calls
           to its destructor until numStudents WATCards are delivered. */
        _Accept(~Groupoff)
        {
            for (WATCard *card : deliveredWATCards)
            {
                delete card;
            }

            break;
        }
        _When(cardDelivered < numStudents) _Else
        {
            yield(groupoffDelay);

            WATCard *wc = new WATCard();

            wc->deposit(sodaCost);

            fWATCards[cardDelivered].delivery(wc);

            deliveredWATCards.push_back(wc);
        }
    }

    printer.print(Printer::Groupoff, 'F');
}

WATCard::FWATCard Groupoff::giftCard()
{
    WATCard::FWATCard fwc;

    fWATCards.push_back(fwc);

    printer.print(Printer::Groupoff, 'D', sodaCost);

    return fwc;
}

// :<
