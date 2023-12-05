#include "BottlingPlant.h"
#include "Printer.h"
#include "NameServer.h"

#include <iostream>
using namespace std;

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments)
    : printer(prt),
      nameServer(nameServer),
      truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour),
      numVendingMachines(numVendingMachines),
      maxShippedPerFlavour(maxShippedPerFlavour),
      maxStockPerFlavour(maxStockPerFlavour),
      timeBetweenShipments(timeBetweenShipments),
      isShuttingDown(false) {}

BottlingPlant::~BottlingPlant()
{
    // cout << "IN CALL TO DESTRUCTOR IN BOTTLING PLANT" << endl;

    printer.print(Printer::BottlingPlant, 'F');
}

void BottlingPlant::main()
{
    printer.print(Printer::BottlingPlant, 'S');

    for (;;)
    {
        yield(timeBetweenShipments);

        for (unsigned int idx = 0; idx < NUM_OF_FLAVOURS; idx += 1)
            productionRun[idx] = prng(maxShippedPerFlavour);

        unsigned int bottlesInProdRun = productionRun[0] + productionRun[1] + productionRun[2] + productionRun[3];

        printer.print(Printer::BottlingPlant, 'G', bottlesInProdRun);
        
        // cout << "BEFORE CALLS TO ACCEPTS IN BOTTLING PLANT" << endl;
        _Accept(~BottlingPlant)
        {
            // cout << "AFTER CALL TO GET DESTRUCTOR IN BOTTLING PLANT" << endl;

            isShuttingDown = true;

            _Accept(getShipment);

            break;
        }
        or _Accept(getShipment)
        {
            // cout << "AFTER CALL TO GET SHIPMENT IN BOTTLING PLANT" << endl;
        }
    }
}

void BottlingPlant::getShipment(unsigned int cargo[])
{
    // cout << "IN CALL TO GET SHIPMENT IN BOTTLING PLANT" << endl;

    if (isShuttingDown)
    {
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