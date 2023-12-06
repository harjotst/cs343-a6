#include "NameServer.h"
#include "Printer.h"
#include "VendingMachine.h"

using namespace std;

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents)
    : printer(prt), numOfVendingMachines(numVendingMachines), numOfStudents(numStudents)
{
    vendingMachines = new VendingMachine *[numOfVendingMachines];

    nextAvailVMSpot = 0;

    nextVendingMachineForStudent = new unsigned int[numOfStudents];

    for (unsigned int idx = 0; idx < numOfStudents; idx += 1)
        nextVendingMachineForStudent[idx] = idx % numOfVendingMachines;
}

NameServer::~NameServer()
{
    delete[] vendingMachines;

    delete[] nextVendingMachineForStudent;

    printer.print(Printer::NameServer, 'F');
}

void NameServer::main()
{
    printer.print(Printer::NameServer, 'S');

    for (;;)
    {
        _Accept(~NameServer)
        {
            break;
        }
        or _Accept(VMregister);
        or _When(nextAvailVMSpot >= numOfVendingMachines) _Accept(getMachine);
        or _When(nextAvailVMSpot >= numOfVendingMachines) _Accept(getMachineList);
    }
}

void NameServer::VMregister(VendingMachine *vendingmachine)
{
    printer.print(Printer::NameServer, 'R', vendingmachine->getId());

    vendingMachines[nextAvailVMSpot++] = vendingmachine;
}

VendingMachine *NameServer::getMachine(unsigned int id)
{
    VendingMachine *vendingMachine = vendingMachines[nextVendingMachineForStudent[id]];

    printer.print(Printer::NameServer, 'N', id, vendingMachine->getId());

    /* Set next VendingMachine for this Student. */
    nextVendingMachineForStudent[id] = (nextVendingMachineForStudent[id] + 1) % numOfVendingMachines;

    return vendingMachine;
}

VendingMachine **NameServer::getMachineList()
{
    return vendingMachines;
}