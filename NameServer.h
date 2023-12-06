#ifndef NAME_SERVER_H
#define NAME_SERVER_H

_Monitor Printer;

_Task VendingMachine;

_Task NameServer
{
private:
    Printer & printer;

    unsigned int numOfVendingMachines, numOfStudents;

    VendingMachine **vendingMachines;

    /* Used as index to next available spot in vendingMachine array
       for VendingMachine provided in call to VMRegister. */
    unsigned int nextAvailVMSpot;

    /* Used to keep track of next vending machine to provide student
       when call to getMachine. */
    unsigned int *nextVendingMachineForStudent;

    void main();

public:
    NameServer(Printer & prt, unsigned int numVendingMachines, unsigned int numStudents);

    ~NameServer();

    /* Register a vending machine in the next available spot 
       in the list of vending machines. */
    void VMregister(VendingMachine *vendingmachine);

    /* Returns a pointer to the next vending machine for student "id". */
    VendingMachine *getMachine(unsigned int id) __attribute__((warn_unused_result));

    /* Returns a list of pointers to all the vending machines */
    VendingMachine **getMachineList() __attribute__((warn_unused_result));
};

#endif // NAME_SERVER_H