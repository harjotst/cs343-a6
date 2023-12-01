#ifndef NAME_SERVER_H
#define NAME_SERVER_H

#include "printer.h"
#include "vendingMachine.h"

_Task NameServer
{
private:
    Printer & printer;

    unsigned int numOfVendingMachines, numOfStudents;

    VendingMachine **vendingMachines;

    unsigned int nextAvailVMSpot;

    unsigned int *nextVendingMachineForStudent;

    VendingMachine *requestedVendingMachine;

    void main(); // done

public:
    NameServer(Printer & prt, unsigned int numVendingMachines, unsigned int numStudents); // done

    /* Register a vending machine in the next available spot in the list of vending machines */
    void VMregister(VendingMachine *vendingmachine); // done

    /* This routine returns a pointer to the vending machine at the specified id */
    VendingMachine *getMachine(unsigned int id) __attribute__((warn_unused_result)); // done

    /* This routine returns a list of pointers to all the vending machines */
    VendingMachine **getMachineList() __attribute__((warn_unused_result)); // done
};

#endif // NAME_SERVER_H