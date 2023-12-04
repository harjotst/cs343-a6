#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

_Monitor Printer
{
private:
    unsigned int numOfStudents, numOfVendingMachines, numOfCouriers;

public:
    enum Kind
    {
        Parent,        // 0
        Groupoff,      // 1
        WATCardOffice, // 2
        NameServer,    // 3
        Truck,         // 4
        BottlingPlant, // 5
        Student,       // 6
        Vending,       // 7
        Courier        // 8
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

        PrinterState(Kind kind, char state, unsigned int value1 = 0, unsigned int value2 = 0, unsigned int lid = 0);
    };

    PrinterState **printerBuffer;

    unsigned int getOffset(Kind kind, unsigned int lid = 0);

    void flush(Kind kind, unsigned int lid = 0);

    void handleOutput(PrinterState * ps);

    void printState(PrinterState * ps);
    void printStateSingle(PrinterState * ps);
    void printStateDouble(PrinterState * ps);

    void handleParentOutput(PrinterState * ps);
    void handleGroupoffOutput(PrinterState * ps);
    void handleWATCardOfficeOutput(PrinterState * ps);
    void handleNameServerOutput(PrinterState * ps);
    void handleTruckOutput(PrinterState * ps);
    void handleBottlingPlantOutput(PrinterState * ps);
    void handleStudentOutput(PrinterState * ps);
    void handleVendingOutput(PrinterState * ps);
    void handleCourierOutput(PrinterState * ps);
};

#endif // PRINTER_H