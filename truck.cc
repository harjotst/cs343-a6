#include "truck.h"

Truck::Truck(Printer &prt,
             NameServer &nameServer,
             BottlingPlant &plant,
             unsigned int numVendingMachines,
             unsigned int maxStockPerFlavour)
    : printer(printer),
      nameServer(nameServer),
      bottlingPlant(plant),
      numOfVendingMachines(numVendingMachines),
      maxStockPerFlavour(maxStockPerFlavour) {}

bool Truck::isCargoEmpty(int cargo[4])
{
    return cargo[0] == 0 && cargo[1] == 0 && cargo[2] == 0 && cargo[3] == 0;
}

void Truck::restock(unsigned int *inventory, unsigned int cargo[4])
{
    for (unsigned int idx = 0; idx < BottlingPlant::NUM_OF_FLAVOURS; idx += 1)
    {
        unsigned int bottlesTransferred = min(maxStockPerFlavour - inventory[idx], cargo[idx]);

        inventory[idx] += bottlesTransferred;

        cargo[idx] -= bottlesTransferred;
    }
}

void Truck::main()
{
    VendingMachine **vendingMachines = nameServer.getMachineList();

    int nextToRestock = 0;

    try
    {
        for (;;)
        {
            yield(1 + prng(9));

            unsigned int cargo[4] = {0, 0, 0, 0};

            bottlingPlant.getShipment(cargo);

            int firstRestocked = nextToRestock;

            for (;;)
            {
                if (isCargoEmpty(cargo))
                    break;

                VendingMachine *vendingMachine = vendingMachines[nextToRestock];

                restock(vendingMachine->inventory(), cargo);

                nextToRestock = (nextToRestock + 1) % numOfVendingMachines;

                if (nextToRestock == firstRestocked)
                    break;
            }

            if (prng(100) == 0)
                yield(10);
        }
    }
    _CatchResume(Shutdown &) {}
}