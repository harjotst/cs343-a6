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
    // cout << "HERE!!!!" << endl;

    printer.print(Printer::Truck, 'S');

    // cout << "AFTER HERE!!!! BEFORE CALLING MACHINE LIST " << &nameServer << endl;

    VendingMachine **vendingMachines = nameServer.getMachineList();

    // cout << "AFTER CALLING GET MACHINE LIST!!!" << endl;

    int nextToRestock = 0;

    try
    {
        for (;;)
        {
            // cout << "BEFORE YIELD!!!!" << endl;

            yield(1 + prng(9));

            unsigned int cargo[4] = {0, 0, 0, 0};

            // cout << "Cargo After Shipment: " << cargo[0] << " " << cargo[1] << " " << cargo[2] << " " << cargo[3] << endl;

            // cout << "BEFORE GET SHIPMENT IN TRUCK" << endl;

            bottlingPlant.getShipment(cargo);

            // cout << "AFTER GET SHIPMENT IN TRUCK" << endl;

            // cout << "Cargo After Shipment: " << cargo[0] << " " << cargo[1] << " " << cargo[2] << " " << cargo[3] << endl;

            unsigned int numOfRemBottles = cargo[0] + cargo[1] + cargo[2] + cargo[3];

            // cout << "NUM OF REM BOTTLES?: " << numOfRemBottles << endl;

            printer.print(Printer::Truck, 'P', numOfRemBottles);

            // exit(1);

            int firstRestocked = nextToRestock;

            for (;;)
            {
                if (isCargoEmpty(cargo))
                    break;

                // cout << "TRUCK IN HERE?" << endl;

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

            // cout << "TRUCK IS DONE ANOTHER THINGY" << endl;
        }
    }
    _Catch(BottlingPlant::Shutdown &)
    {
    }
}