#include "student.h"

Student::Student(Printer &prt,
                 NameServer &nameServer,
                 WATCardOffice &cardOffice,
                 Groupoff &groupoff,
                 unsigned int id,
                 unsigned int maxPurchases)
    : printer(prt),
      nameServer(nameServer),
      cardOffice(cardOffice),
      groupoff(groupoff),
      id(id),
      maxPurchases(maxPurchases) {}

void Student::main()
{
    unsigned int bottlesToPurchase = prng(1, maxPurchases),
                 favouriteFlavour = prng(3);

    WATCard::FWATCard fwc = cardOffice.create(id, 5), fgc = groupoff.giftCard();

    VendingMachine *vendingMachine = nameServer.getMachine(id);

    bool skipYield = false;

    for (unsigned int timesPurchased = 0; cnt < maxPurchases;)
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
            }
            or _Select(fwc)
            {
                watCard = fwc();

                vendingMachine->buy(favouriteFlavour, *watCard);
            }

            timesPurchased += 1;
        }
        _Catch(VendingMachine::Free &)
        {
            if (prng(1) == 0)
                yield(4);
        }
        _Catch(VendingMachine::Funds &)
        {
            cardOffice.transder(id, 5 + vendingMachine->cost(), watCard);
        }
        _Catch(VendingMachine::Stock &)
        {
            vendingMachine = nameServer.getMachine(id);
        }
        _Catch(WATCardOffice::Lost &)
        {
            watCard = cardOffice.create(id, 5);

            skipYield = true;
        }
    }
}