#ifndef BOTTLING_PLANT_H
#define BOTTLING_PLANT_H

#include "Truck.h"

_Monitor Printer;

_Task NameServer;

_Task BottlingPlant
{
private:
    Printer & printer;

    NameServer & nameServer;

    Truck truck;

    unsigned int numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments;

    bool isShuttingDown;

    unsigned int productionRun[4];

    void main();

public:
    enum Flavours
    {
        BLUES_BLACK_CHERRY, // 0
        CLASSIC_CREAM_SODA, // 1
        ROCK_ROOT_BEER,     // 2
        JAZZ_LIME,          // 3
        NUM_OF_FLAVOURS     // 4
    };

    /* Exception thrown when bottling plant shuts down. Main purpose is to tell Truck to shutdown */
    _Event Shutdown{};

    BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                  unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                  unsigned int timeBetweenShipments); // done

    ~BottlingPlant();

    /* Called by truck to get bottle cargo that needs to be distributed to vending machines */
    void getShipment(unsigned int cargo[]); // done
};

#endif // BOTTLING_PLANT_H