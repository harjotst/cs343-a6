#ifndef TRUCK_H
#define TRUCK_H

#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "vendingMachine.h"

_Task Truck
{
private:
    Printer &printer;

    NameServer &nameServer;

    BottlingPlant &bottlingPlant;

    bool isCargoEmpty(int cargo[4]);

    void main();

public:
    Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
          unsigned int numVendingMachines, unsigned int maxStockPerFlavour); // done
}

#endif // TRUCK_H