#include "BottlingPlant.h"
#include "Printer.h"
#include "NameServer.h"

#include <iostream>
using namespace std;

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments)
    : printer(prt), nameServer(nameServer), truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour),
      numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
      maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments), isShuttingDown(false) {}

BottlingPlant::~BottlingPlant()
{
    printer.print(Printer::BottlingPlant, 'F');
}

void BottlingPlant::main()
{
    printer.print(Printer::BottlingPlant, 'S');

    for (;;)
    {
        yield(timeBetweenShipments);

        for (unsigned int idx = 0; idx < NUM_OF_FLAVOURS; idx += 1)
        {
            productionRun[idx] = prng(maxShippedPerFlavour);
        }

        unsigned int bottlesInProdRun = productionRun[0] + productionRun[1] + productionRun[2] + productionRun[3];

        printer.print(Printer::BottlingPlant, 'G', bottlesInProdRun);
        
        _Accept(~BottlingPlant)
        {
            /* Time to shutdown. Notify Truck. */
            isShuttingDown = true;

            /* Wait for/accept a call to getShipment so the Truck can
               catch the Shutdown exception. */
            _Accept(getShipment);

            break;
        }
        or _Accept(getShipment);
    }
}

void BottlingPlant::getShipment(unsigned int cargo[])
{
    if (isShuttingDown)
    {
        /* Used to stop BottlingPlant from throwing RendezvousFailure
           as Rendezvous doesn't complete from the perspective of the
           BottlingPlant. Found usage of this method in uC++ Reference
           Manual page 101. */
        uRendezvousAcceptor();

        _Throw Shutdown();
    }
    else
    {
        for (unsigned int idx = 0; idx < NUM_OF_FLAVOURS; idx += 1)
            cargo[idx] = productionRun[idx];

        printer.print(Printer::BottlingPlant, 'P');
    }
}