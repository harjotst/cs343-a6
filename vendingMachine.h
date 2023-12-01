#ifndef VENDING_MACHINE_H
#define VENDING_MACHINE_H

#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "watcard.h"

_Task VendingMachine
{
private:
    Printer &printer;

    NameServer &nameServer;

    WATCardOffice &cardOffice;

    Groupoff &groupoff;

    unsigned int id, sodaCost;

    unsigned int *inv;

    void main();

public:
    /* Exception thrown when student gets free soda by watching advertisement */
    _Event Free{};

    /* Exception thrown when student has insufficent funds to buy bottle */
    _Event Funds{};

    /* Exception thrown when flavour requested for purchase is out of stock */
    _Event Stock{}; 

    VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost); // done

    /* Attempts to buy bottle of specified flavour and withdraws cost from provided card */
    void buy(BottlingPlant::Flavours flavour, WATCard & card); // done

    /* Called by truck to retrieve a reference to a vending machine's inventory */
    unsigned int *inventory() __attribute__((warn_unused_result)); // done

    /* Called once a Truck has restocked the vending machine's inventory */
    void restocked(); // done

    /* Return the cost of a bottle in this vending machine */
    _Nomutex unsigned int cost() const; // done

    /* Return the id of this vending machine */
    _Nomutex unsigned int getId() const; // done
};

#endif // VENDING_MACHINE_H