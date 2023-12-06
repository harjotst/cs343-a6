#include "Truck.h"
#include "Printer.h"
#include "NameServer.h"
#include "BottlingPlant.h"
#include "VendingMachine.h"
#include <iostream>

using namespace std;

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour)
    : printer(prt), nameServer(nameServer), bottlingPlant(plant), numOfVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {}

Truck::~Truck()
{
    printer.print(Printer::Truck, 'F');
}

bool Truck::isCargoEmpty(unsigned int cargo[4])
{
    return cargo[0] == 0 && cargo[1] == 0 && cargo[2] == 0 && cargo[3] == 0;
}

void Truck::restock(unsigned int vid, unsigned int *inventory, unsigned int cargo[4])
{
    unsigned int notFilled = 0;

    for (unsigned int idx = 0; idx < BottlingPlant::NUM_OF_FLAVOURS; idx += 1)
    {
        unsigned int bottlesTransferred = min(maxStockPerFlavour - inventory[idx], cargo[idx]);

        inventory[idx] += bottlesTransferred;

        cargo[idx] -= bottlesTransferred;

        notFilled += maxStockPerFlavour - inventory[idx];
    }

    if (notFilled != 0)
        printer.print(Printer::Truck, 'U', vid, notFilled);
}

void Truck::main()
{
    printer.print(Printer::Truck, 'S');

    VendingMachine **vendingMachines = nameServer.getMachineList();

    int nextToRestock = 0;

    try
    {
        for (;;)
        {
            yield(1 + prng(9));

            unsigned int cargo[4] = {0, 0, 0, 0};

            bottlingPlant.getShipment(cargo);

            unsigned int numOfRemBottles = cargo[0] + cargo[1] + cargo[2] + cargo[3];

            printer.print(Printer::Truck, 'P', numOfRemBottles);

            int firstRestocked = nextToRestock;

            for (;;)
            {
                if (isCargoEmpty(cargo))
                    break;

                VendingMachine *vendingMachine = vendingMachines[nextToRestock];

                printer.print(Printer::Truck, 'd', vendingMachine->getId(), numOfRemBottles);

                restock(vendingMachine->getId(), vendingMachine->inventory(), cargo);

                vendingMachine->restocked();

                numOfRemBottles = cargo[0] + cargo[1] + cargo[2] + cargo[3];

                printer.print(Printer::Truck, 'D', vendingMachine->getId(), numOfRemBottles);

                nextToRestock = (nextToRestock + 1) % numOfVendingMachines;

                if (nextToRestock == firstRestocked)
                    break;
            }

            if (prng(100) == 0)
            {
                printer.print(Printer::Truck, 'W');

                yield(10);
            }
        }
    }
    _Catch(BottlingPlant::Shutdown &)
    {
    }
}
