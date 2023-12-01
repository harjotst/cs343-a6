#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

using namespace std;

_Monitor
{
private:
    unsigned int numOfStudents, numOfVendingMachines, numOfCouriers;

    void flush(Kind kind, unsigned int lid = 0);

    void handleOutput(PrinterState *ps);

    void handleParentOutput(PrinterState *ps);
    void handleGroupoffOutput(PrinterState *ps);
    void handleWATCardOfficeOutput(PrinterState *ps);
    void handleNameServerOutput(PrinterState *ps);
    void handleTruckOutput(PrinterState *ps);
    void handleBottlingPlantOutput(PrinterState *ps);
    void handleStudentOutput(PrinterState *ps);
    void handleVendingOutput(PrinterState *ps);
    void handleCourierOutput(PrinterState *ps);

public:
    enum Kind
    {
        Parent,
        Groupoff,
        WATCardOffice,
        NameServer,
        Truck,
        BottlingPlant,
        Student,
        Vending,
        Courier
    };

    Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers); // done

    /* This method is used by single instance objects to print single state updates */
    void print(Kind kind, char state);

    /* This method is used by single instance objects to print state updates with one parameter */
    void print(Kind kind, char state, unsigned int value1);

    /* This method is used by single instance objects to print state updates with two parametersa */
    void print(Kind kind, char state, unsigned int value1, unsigned int value2);

    /* This method is used by multiple instance objects to print single state updates */
    void print(Kind kind, unsigned int lid, char state);

    /* This method is used by multiple instance objects to print state updates with one parameter */
    void print(Kind kind, unsigned int lid, char state, unsigned int value1);

    /* This method is used by multiple instance objects to print state updates with two parameters */
    void print(Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2);

private:
    struct PrinterState
    {
        Kind kind;
        char state;
        unsigned int value1, value2, lid;

        PrinterState(Kind kind,
                     char state,
                     unsigned int value1 = 0,
                     unsigned int value2 = 0,
                     unsigned int lid = 0);
    };

    PrinterState **printerBuff;
};

#endif // PRINTER_H