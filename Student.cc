#include "Student.h"
#include "Printer.h"
#include "BottlingPlant.h"
#include "NameServer.h"
#include "VendingMachine.h"
#include "WATCardOffice.h"
#include "Groupoff.h"

#include <iostream>
using namespace std;

Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice,
                 Groupoff &groupoff, unsigned int id, unsigned int maxPurchases)
    : printer(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff),
      id(id), maxPurchases(maxPurchases) {}

Student::~Student()
{
    printer.print(Printer::Student, id, 'F');
}

void Student::main()
{
    unsigned int bottlesToPurchase = prng(1, maxPurchases);

    BottlingPlant::Flavours favouriteFlavour = (BottlingPlant::Flavours)prng(3);

    printer.print(Printer::Student, id, 'S', favouriteFlavour, bottlesToPurchase);

    WATCard::FWATCard fwc = cardOffice.create(id, 5), fgc = groupoff.giftCard();

    VendingMachine *vendingMachine = nameServer.getMachine(id);

    printer.print(Printer::Student, id, 'V', vendingMachine->getId());

    bool skipYield = false;

    for (unsigned int sodasPurchased = 0; sodasPurchased < bottlesToPurchase;)
    {
        if (!skipYield)
            yield(prng(1, 10));

        skipYield = false;

        WATCard *watCard = nullptr, *giftCard = nullptr;

        try
        {
            _Select(fgc)
            {
                giftCard = fgc();

                vendingMachine->buy(favouriteFlavour, *giftCard);

                fgc.reset();

                printer.print(Printer::Student, id, 'G', favouriteFlavour, giftCard->getBalance());
            }
            or _Select(fwc)
            {
                watCard = fwc();

                vendingMachine->buy(favouriteFlavour, *watCard);

                printer.print(Printer::Student, id, 'B', favouriteFlavour, watCard->getBalance());

                sodasPurchased += 1;
            }
        }
        _Catch(VendingMachine::Free &)
        {
            if (giftCard != nullptr)
            {
                printer.print(Printer::Student, id, 'a', favouriteFlavour, giftCard->getBalance());

                fgc.reset();
            }
            else
                printer.print(Printer::Student, id, 'A', favouriteFlavour, watCard->getBalance());

            if (prng(1) == 0)
                yield(4);
            else
                printer.print(Printer::Student, id, 'X');
        }
        _Catch(VendingMachine::Funds &)
        {
            fwc = cardOffice.transfer(id, 5 + vendingMachine->cost(), watCard);
        }
        _Catch(VendingMachine::Stock &)
        {
            vendingMachine = nameServer.getMachine(id);

            printer.print(Printer::Student, id, 'V', vendingMachine->getId());
        }
        _Catch(WATCardOffice::Lost &)
        {
            printer.print(Printer::Student, id, 'L');

            fwc = cardOffice.create(id, 5);

            skipYield = true;
        }
    }
}
