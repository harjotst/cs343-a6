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

    /* Flag used to "signal" Truck when it's time to shutdown. */
    bool isShuttingDown;

    /* Used to hold number of bottles of each flavour after
       production run has completed. */
    unsigned int productionRun[4];

    void main();

public:
    BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                  unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                  unsigned int timeBetweenShipments);

    ~BottlingPlant();

    /* The different flavours of Cola and the number of total flavours. */
    enum Flavours
    {
        BLUES_BLACK_CHERRY, // 0
        CLASSIC_CREAM_SODA, // 1
        ROCK_ROOT_BEER,     // 2
        JAZZ_LIME,          // 3
        NUM_OF_FLAVOURS     // 4
    };

    /* Exception thrown at Truck when bottling plant shuts down so Truck
       may shutdown as well. */
    _Event Shutdown{};

    /* Called by truck after production run to get cola cargo that needs 
       to be distributed to vending machines. */
    void getShipment(unsigned int cargo[]);
};

#endif // BOTTLING_PLANT_H