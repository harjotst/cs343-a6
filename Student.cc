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
      id(id), maxPurchases(maxPurchases)
{
    name = "Student" + to_string(id);

    setName(name.c_str());
}

Student::~Student()
{
    printer.print(Printer::Student, id, 'F');
}

void Student::main()
{
    unsigned int bottlesToPurchase = prng(1, maxPurchases);

    BottlingPlant::Flavours favouriteFlavour = (BottlingPlant::Flavours)prng(3);

    cout << "STUDENT " << id << " STARTING WITH VALUES: " << bottlesToPurchase << " " << favouriteFlavour << endl;

    printer.print(Printer::Student, id, 'S', favouriteFlavour, bottlesToPurchase);

    cout << "STUDENT " << id << " BEFORE GETTING FWC AND FGC" << endl;

    WATCard::FWATCard fwc = cardOffice.create(id, 5), fgc = groupoff.giftCard();

    cout << "STUDENT " << id << " AFTER GETTING FWC AND FGC" << endl;

    cout << "STUDENT " << id << " BEFORE GETTING VENDING MACHINE" << endl;

    VendingMachine *vendingMachine = nameServer.getMachine(id);

    cout << "STUDENT " << id << " AFTER GETTING VENDING MACHINE " << vendingMachine->getId() << endl;

    printer.print(Printer::Student, id, 'V', vendingMachine->getId());

    bool skipYield = false;

    for (unsigned int sodasPurchased = 0; sodasPurchased < maxPurchases;)
    {
        if (!skipYield)
            yield(prng(1, 10));

        skipYield = false;

        WATCard *watCard = nullptr, *giftCard = nullptr;

        try
        {
            _Select(fgc)
            {
                cout << "STUDENT " << id << " BEFORE ACCESSING FGC" << endl;

                giftCard = fgc();

                cout << "STUDENT " << id << " AFTER ACCESSING FGC" << endl;

                cout << "STUDENT " << id << " BEFORE BUYING WITH FGC BALANCE " << giftCard->getBalance() << endl;

                vendingMachine->buy(favouriteFlavour, *giftCard);

                cout << "STUDENT " << id << " AFTER BUYING WITH FGC" << endl;

                cout << "STUDENT " << id << " BEFORE RESETTING FGC" << endl;

                fgc.reset();

                cout << "STUDENT " << id << " AFTER RESETTING FGC" << endl;

                printer.print(Printer::Student, id, 'G', favouriteFlavour, giftCard->getBalance());
            }
            or _Select(fwc)
            {
                cout << "STUDENT " << id << " BEFORE ACCESSING FWC" << endl;

                watCard = fwc();

                cout << "STUDENT " << id << " AFTER ACCESSING FWC" << endl;

                cout << "STUDENT " << id << " BEFORE BUYING WITH FWC BALANCE " << watCard->getBalance() << endl;

                vendingMachine->buy(favouriteFlavour, *watCard);

                cout << "STUDENT " << id << " AFTER BUYING WITH FWC" << endl;

                printer.print(Printer::Student, id, 'B', favouriteFlavour, watCard->getBalance());
            }

            sodasPurchased += 1;

            cout << "STUDENT " << id << " NUM OF SODAS PURCHASED INCREASED TO " << sodasPurchased << endl;
        }
        _Catch(VendingMachine::Free &)
        {
            cout << "STUDENT " << id << " GOT FREE BOTTLE" << endl;

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
            cout << "STUDENT " << id << " LACK OF FUNDS" << endl;

            cout << "STUDENT " << id << " BEFORE TRANSFERING MONEY INTO FWC" << endl;

            fwc = cardOffice.transfer(id, 5 + vendingMachine->cost(), watCard);

            cout << "STUDENT " << id << " AFTER TRANSFERING MONEY INTO FWC" << endl;
        }
        _Catch(VendingMachine::Stock &)
        {
            cout << "STUDENT " << id << " INSUFFICENT STOCK" << endl;

            cout << "STUDENT " << id << " BEFORE GETTING NEW VENDING MACHINE" << endl;

            vendingMachine = nameServer.getMachine(id);

            cout << "STUDENT " << id << " AFTER GETTING NEW VENDING MACHINE " << vendingMachine->getId() << endl;

            printer.print(Printer::Student, id, 'V', vendingMachine->getId());
        }
        _Catch(WATCardOffice::Lost &)
        {
            cout << "STUDENT " << id << " GOT FREE BOTTLE" << endl;

            printer.print(Printer::Student, id, 'L');

            cout << "STUDENT " << id << " BEFORE GETTING NEW WATCARD" << endl;

            if (watCard != nullptr)
                fwc = cardOffice.create(id, 5);

            cout << "STUDENT " << id << " AFTER GETTING NEW WATCARD" << endl;

            skipYield = true;
        }
    }
}