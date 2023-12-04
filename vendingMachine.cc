#include "vendingMachine.h"

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost)
    : printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost)
{
    inv = new unsigned int[BottlingPlant::NUM_OF_FLAVOURS];

    for (unsigned int idx = 0; idx < BottlingPlant::NUM_OF_FLAVOURS; idx += 1)
        inv[idx] = 0;

    nameServer.VMregister(this);
}

VendingMachine::~VendingMachine()
{
    printer.print(Printer::Vending, id, 'F');
}

void VendingMachine::main()
{
    printer.print(Printer::Vending, id, 'S');

    bool restocking = false;

    for (;;)
    {
        _When(restocking) _Accept(restocked) { restocking = false; }
        or _When(!restocking) _Accept(inventory) { restocking = true; }
        or _When(!restocking) _Accept(buy);
    }
}

void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard &card)
{
    _Select(card.FWATCard)
    {
        if (card.FWATCard->getBalance() < sodaCost)
            _Throw Funds();

        if (inv[flavour] == 0)
            _Throw Stock();

        if (prng(5) == 0)
        {
            printer.print(Printer::Vending, id, 'A');

            _Throw Free();
        }

        card.FWATCard->withdraw(sodaCost);

        printer.print(Printer::Vending, id, 'B', flavour);
    }
}

unsigned int *VendingMachine::inventory()
{
    printer.print(Printer::Vending, id, 'r');

    return inv;
}

void VendingMachine::restocked()
{
    printer.print(Printer::Vending, id, 'R');
}

unsigned int VendingMachine::cost()
{
    printer.print(Printer::Vending, id, 'R');

    return sodaCost;
}

unsigned int VendingMachine::getId()
{
    return id;
}
