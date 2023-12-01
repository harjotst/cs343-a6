#include "nameServer.h"

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines,
                       unsigned int numStudents)
    : printer(prt), numOfVendingMachines(numVendingMachines), numOfStudents(numStudents)
{
    vendingMachines = new VendingMachine *[numOfVendingMachines];

    nextAvailVMSpot = 0;

    nextVendingMachineForStudent = new (unsigned int)[numOfStudents];

    for (unsigned int idx = 0; idx < numOfStudents; idx += 1)
        nextVendingMachineForStudent[idx] = idx % numOfVendingMachines;
}

NameServer::~NameServer() {}

void NameServer::main()
{
    for (; nextAvailVMSpot < numOfVendingMachines; nextAvailVMSpot += 1)
        Accept(VMregister);

    for (;;)
    {
        _Accept(~NameServer) break;
        or _Accept(getMachine){} 
        or _Accept(getMachineList) {}
    }
}

void NameServer::VMregister(VendingMachine *vendingmachine)
{
    vendingMachines[nextAvailVMSpot] = vendingmachine;
}

VendingMachine *NameServer::getMachine(unsigned int id)
{
    VendingMachine vendingMachine = nextVendingMachineForStudent[id];

    nextVendingMachineForStudent[id] = (nextVendingMachineForStudent[id] + 1) % numOfVendingMachines;

    return vendingMachine;
}

VendingMachine **NameServer::getMachineList()
{
    return vendingMachines;
}