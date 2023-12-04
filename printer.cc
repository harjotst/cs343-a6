#include "printer.h"

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers)
    : numOfStudents(numStudent), numOfVendingMachines(numVendingMachines), numOfCouriers(numCouriers)
{
    unsigned int buffSize = 6 + numOfStudents + numOfVendingMachines + numOfCouriers;

    printerBuffer = new PrinterState *[buffSize];

    for (unsigned int idx = 0; idx < buffSize; idx += 1)
        printerBuffer[idx] = nullptr;
}

Printer::PrinterState::PrinterState(Kind kind, char state, unsigned int value1 = 0, unsigned int value2 = 0, unsigned int lid = 0)
    : kind(kind), state(state), value1(value1), value2(value2), lid(lid) {}

void Printer::handleParentOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'D')
        printStateDouble(ps);
}

void Printer::handleGroupoffOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'D')
        printStateSingle(ps);
}

void Printer::handleWATCardOfficeOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'W' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'C' || ps->state == 'T')
        printStateDouble(ps);
}

void Printer::handleNameServerOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'R')
        printStateSingle(ps);
    else if (ps->state == 'N')
        printStateDouble(ps);
}

void Printer::handleTruckOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'W' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'P')
        printStateSingle(ps);
    else if (ps->state == 'd' || ps->state == 'U' || ps->state == 'D')
        printStateDouble(ps);
}

void Printer::handleBottlingPlantOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'P' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'G')
        printStateSingle(ps);
}

void Printer::handleStudentOutput(PrinterState *ps)
{
    if (ps->state == 'X' || ps->state == 'L' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'V')
        printStateSingle(ps);
    else if (ps->state == 'S' || ps->state == 'G' || ps->state == 'B' || ps->state == 'a' || ps->state == 'A')
        printStateDouble(ps);
}

void Printer::handleVendingOutput(PrinterState *ps)
{
    if (ps->state == 'r' || ps->state == 'B' || ps->state == 'A' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'S')
        printStateSingle(ps);
    else if (ps->state == 'B')
        printStateDouble(ps);
}

void Printer::handleCourierOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'F')
        printState(ps);
    else if (ps->state == 'L')
        printStateSingle(ps);
    else if (ps->state == 't' || ps->state == 'T')
        printStateDouble(ps);
}

void Printer::handleOutput(PrinterState *ps)
{
    if (ps == nullptr)
        cout << "\t";
    else if (ps->kind == Parent)
        handleParentOutput(ps);
    else if (ps->kind == Groupoff)
        handleGroupoffOutput(ps);
    else if (ps->kind == WATCardOffice)
        handleWATCardOfficeOutput(ps);
    else if (ps->kind == NameServer)
        handleNameServerOutput(ps);
    else if (ps->kind == Truck)
        handleTruckOutput(ps);
    else if (ps->kind == BottlingPlant)
        handleBottlingPlantOutput(ps);
    else if (ps->kind == Student)
        handleStudentOutput(ps);
    else if (ps->kind == Vending)
        handleVendingOutput(ps);
    else if (ps->kind == Courier)
        handleCourierOutput(ps);
}

unsigned int Printer::getOffset(Kind kind, unsigned int lid)
{
    return kind == Student   ? Student + lid
           : kind == Vending ? Student + numOfStudents + lid
           : kind == Courier ? Student + numOfStudents + numOfVendingMachines + lid
                             : kind;
}

void Printer::flush(Kind kind, unsigned int lid)
{
    if (printerBuffer[getOffset(kind, lid)] == nullptr)
        return;

    unsigned int buffSize = 6 + numOfStudents + numOfVendingMachines + numOfCouriers;

    for (unsigned int idx = 0; idx < buffSize; idx += 1)
        handleOutput(printerBuffer[idx]);

    cout << endl;
}

void Printer::printState(PrinterState *ps)
{
    cout << ps->state << "       ";
}

void Printer::printStateSingle(PrinterState *ps)
{
    unsigned int digits = ps->value1 > 0 ? floor(log10(ps->value1) + 1) : 1;

    cout << ps->state << " " << ps->value1 << string(6 - digits, ' ');
}

void Printer::printStateDouble(PrinterState *ps)
{
    unsigned int digits1 = ps->value1 > 0 ? floor(log10(ps->value1) + 1) : 1;

    unsigned int digits2 = ps->value2 > 0 ? floor(log10(ps->value2) + 1) : 1;

    cout << ps->state << " " << ps->value1 << "," << ps->value2 << string(5 - digits1 - digits2, ' ');
}

void Printer::print(Kind kind, char state)
{
    flush(kind);

    printerBuffer[getOffset(kind)] = new PrinterState(kind, state);
}

void Printer::print(Kind kind, char state, unsigned int value1)
{
    flush(kind);

    printerBuffer[getOffset(kind)] = new PrinterState(kind, state, value1);
}

void Printer::print(Kind kind, char state, unsigned int value1, unsigned int value2)
{
    flush(kind);

    printerBuffer[getOffset(kind)] = new PrinterState(kind, state, value1, value2);
}

void Printer::print(Kind kind, unsigned int lid, char state)
{
    flush(kind, lid);

    printerBuffer[getOffset(kind, lid)] = new PrinterState(kind, state, 0, 0, lid);
}

void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1)
{
    flush(kind, lid);

    printerBuffer[getOffset(kind, lid)] = new PrinterState(kind, state, value1, 0, lid);
}

void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2)
{
    flush(kind, lid);

    printerBuffer[getOffset(kind, lid)] = new PrinterState(kind, state, value1, value2, lid);
}
