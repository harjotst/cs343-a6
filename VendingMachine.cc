#include "VendingMachine.h"
#include "Printer.h"
#include "NameServer.h"
#include "WATCard.h"
#include <iostream>

using namespace std;

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost)
    : printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost)
{
    inv = new unsigned int[BottlingPlant::NUM_OF_FLAVOURS];

    for (unsigned int idx = 0; idx < BottlingPlant::NUM_OF_FLAVOURS; idx += 1)
        inv[idx] = 0;

    name = "Vending" + to_string(id);

    setName(name.c_str());
}

VendingMachine::~VendingMachine()
{
    printer.print(Printer::Vending, id, 'F');
}

void VendingMachine::main()
{
    nameServer.VMregister(this);

    printer.print(Printer::Vending, id, 'S', sodaCost);

    bool restocking = false;

    for (;;)
    {

        _When(restocking) _Accept(restocked)
        {
            restocking = false;
        }
        or _When(!restocking) _Accept(inventory)
        {
            restocking = true;
        }
        or _When(!restocking) _Accept(buy);
    }
}

void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard &card)
{
    if (card.getBalance() < sodaCost)
    {
        uRendezvousAcceptor();

        _Throw Funds();
    }

    if (inv[(unsigned int)flavour] == 0)
    {
        uRendezvousAcceptor();

        _Throw Stock();
    }

    if (prng(5) == 0)
    {
        printer.print(Printer::Vending, id, 'A');

        uRendezvousAcceptor();

        _Throw Free();
    }

    card.withdraw(sodaCost);

    inv[(unsigned int)flavour] -= 1;

    printer.print(Printer::Vending, id, 'B', flavour, inv[(unsigned int)flavour]);
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

_Nomutex unsigned int VendingMachine::cost() const
{
    printer.print(Printer::Vending, id, 'R');

    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() const
{
    return id;
}
