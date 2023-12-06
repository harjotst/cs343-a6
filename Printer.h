#ifndef PRINTER_H
#define PRINTER_H

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

    ~Printer();

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

    /* Used to get offset index to "lid" of "kind" in printerBuffer. */
    unsigned int getOffset(Kind kind, unsigned int lid = 0);

    /* Used to flush printer buffer's state if index getOffset(kind, lid)
       in the printerBuffer array has a state in it already. */
    void flush(Kind kind, unsigned int lid = 0);

    /* Used to call specifc handleOutput methods based on PrinterState
       type. */
    void handleOutput(PrinterState * ps);

    /* Used to print PrinterState with only state. */
    void printState(PrinterState * ps);

    /* Used to print PrinterState with one value. */
    void printStateSingle(PrinterState * ps);

    /* Used to print PrinterState with two values.  */
    void printStateDouble(PrinterState * ps);

    /* Specific handleOutputs */
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
