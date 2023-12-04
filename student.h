#ifndef STUDENT_H
#define STUDENT_H

_Task Student
{
private:
    Printer & printer;

    NameServer & nameServer;

    WATCardOffice & cardOffice;

    Groupoff *groupoff;

    unsigned int id, maxPurchases;

    void main();

public:
    Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice,
            Groupoff & groupoff, unsigned int id, unsigned int maxPurchases);

    ~Student();
};

#endif // STUDENT_H