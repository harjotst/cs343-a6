#include "printer.h"

Printer::Printer(unsigned int numStudents,
                 unsigned int numVendingMachines,
                 unsigned int numCouriers)
    : numOfStudents(numStudent),
      numOfVendingMachines(numVendingMachines),
      numOfCouriers(numCouriers)
{
    unsigned int buffSize = 6 + numOfStudents + numOfVendingMachines + numOfCouriers;

    printerBuff = new PrinterState *[buffSize];

    for (unsigned int idx = 0; idx < buffSize; idx += 1)
        printerBuff[idx] = nullptr;
}

Printer::PrinterState::PrinterState(Kind kind,
                                    char state,
                                    unsigned int value1 = 0,
                                    unsigned int value2 = 0,
                                    unsigned int lid = 0)
    : kind(kind),
      state(state),
      value1(value1),
      value2(value2),
      lid(lid);

void Printer::handleParentOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'D')
        cout << "D " << ps->value1 << "," << ps->value2 << "   ";
}

void Printer::handleGroupoffOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'D')
        cout << "D " << ps->value1 << "     ";
}

void Printer::handleWATCardOfficeOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'W' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'C' || ps->state == 'T')
        cout << ps->state << " " << ps->value1 << "," << ps->value2 << "   ";
}

void Printer::handleNameServerOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'R')
        cout << "R " << ps->value1 << "     ";
    else if (ps->state == 'N')
        cout << "N " << ps->value1 << "," << ps->value2 << "   ";
}

void Printer::handleTruckOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'W' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'P')
        cout << "P " << ps->value1 << "     ";
    else if (ps->state == 'd' || ps->state == 'U' || ps->state == 'D')
        cout << ps->state << " " << ps->value1 << "," << ps->value2 << "   ";
}

void Printer::handleBottlingPlantOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'P' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'G')
        cout << "G " << ps->value1 << "     ";
}

void Printer::handleStudentOutput(PrinterState *ps)
{
    if (ps->state == 'X' || ps->state == 'L' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'V')
        cout << "V " << ps->value1 << "     ";
    else if (ps->state == 'S' || ps->state == 'G' || ps->state == 'B' || ps->state == 'a' || ps->state == 'A')
        cout << ps->state << " " << ps->value1 << "," << ps->value2 << "   ";
}

void Printer::handleVendingOutput(PrinterState *ps)
{
    if (ps->state == 'r' || ps->state == 'B' || ps->state == 'A' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'S')
        cout << "S " << ps->value1 << "     ";
    else if (ps->state == 'B')
        cout << "B " << ps->value1 << "," << ps->value2 << "   ";
}

void Printer::handleCourierOutput(PrinterState *ps)
{
    if (ps->state == 'S' || ps->state == 'F')
        cout << ps->state << "       " << endl;
    else if (ps->state == 'L')
        cout << "L " << ps->value1 << "     ";
    else if (ps->state == 't' || ps->state == 'T')
        cout << "B " << ps->value1 << "," << ps->value2 << "   ";
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

void Printer::flush(Kind kind, unsigned int lid)
{
    if (printerBuff[kind + lid] == nullptr)
        return;

    unsigned int buffSize = 6 + numOfStudents + numOfVendingMachines + numOfCouriers;

    for (unsigned int idx = 0; idx < buffSize; idx += 1)
        handleOutput(printerBuff[idx]);

    cout << endl;
}

void Printer::print(Kind kind, char state)
{
    flush(kind);

    printerBuff = new PrinterState(kind, state);
}

void Printer::print(Kind kind, char state, unsigned int value1)
{
    flush(kind);

    printerBuff = new PrinterState(kind, state, value1);
}

void Printer::print(Kind kind, char state, unsigned int value1, unsigned int value2)
{
    flush(kind);

    printerBuff = new PrinterState(kind, state, value1, value2);
}

void Printer::print(Kind kind, unsigned int lid, char state)
{
    flush(kind, lid);

    printerBuff = new PrinterState(kind, state, 0, 0, lid);
}

void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1)
{
    flush(kind, lid);

    printerBuff = new PrinterState(kind, state, value1, 0, lid);
}

void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2)
{
    flush(kind, lid);

    printerBuff = new PrinterState(kind, state, value1, value2, lid);
}
