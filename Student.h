#ifndef STUDENT_H
#define STUDENT_H

#include <string>

using namespace std;

_Monitor Printer;

_Task NameServer;

_Task WATCardOffice;

_Task Groupoff;

_Task Student
{
private:
    Printer & printer;

    NameServer & nameServer;

    WATCardOffice & cardOffice;

    Groupoff & groupoff;

    unsigned int id, maxPurchases;

    string name;

    void main();

public:
    Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice,
            Groupoff & groupoff, unsigned int id, unsigned int maxPurchases);

    ~Student();
};

#endif // STUDENT_H