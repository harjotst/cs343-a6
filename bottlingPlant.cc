#include "bottlingPlant.h"

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments)
    : printer(prt), nameServer(nameServer), numVendingMachines(numVendingMachines),
      maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour),
      timeBetweenShipments(timeBetweenShipments),
      truck(prt, nameServer, *self, numVendingMachines, maxStockPerFlavour),
      isShuttingDown(false) {}

BottlingPlant::~BottlingPlant() {}

void BottlingPlant::main()
{
    for (;;)
    {
        yield(timeBetweenShipments);

        Accept(~BottlingPlant)
        {
            isShuttingDown = true;

            Accept(getShipment);

            break;
        }
        or Accept(getShipment)
        {
            for (unsigned int idx = 0; idx < NUM_OF_FLAVOURS; idx += 1)
                productionRun[idx] = prng(maxShippedPerFlavour);
        }
    }
}

void BottlingPlant::getShipment(unsigned int cargo[])
{
    if (isShuttingDown)
    {
        _Resume Shutdown() _At truck;
    }
    else
    {
        for (unsigned int idx = 0; idx < NUM_OF_FLAVOURS; idx += 1)
            cargo[idx] = productionRun[idx];
    }
}