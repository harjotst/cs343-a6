#include "vendingMachine.h"

VendingMachine::VendingMachine(Printer &prt,
                               NameServer &nameServer,
                               unsigned int id,
                               unsigned int sodaCost)
    : printer(prt),
      nameServer(nameServer),
      id(id),
      sodaCost(sodaCost)
{
    inv = new unsigned int[BottlingPlant::NUM_OF_FLAVOURS];

    for (unsigned int idx = 0; idx < BottlingPlant::NUM_OF_FLAVOURS; idx += 1)
        inv[idx] = 0;

    nameServer.VMregister(this);
}

void VendingMachine::main()
{
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
    if (card.getBalance() < sodaCost)
        _Throw Funds();

    if (inv[flavour] == 0)
        _Throw Stock();

    if (prng(5) == 0)
        _Throw Free();

    card.withdraw(sodaCost);
}

unsigned int *VendingMachine::inventory()
{
    return inv;
}

voidVendingMachine::restocked() {}

unsigned int VendingMachine::cost()
{
    return sodaCost;
}

unsigned int VendingMachine::getId()
{
    return id;
}
