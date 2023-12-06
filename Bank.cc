#include "Bank.h"

Bank::Bank(unsigned int numStudents)
{
    studentAccountBalances = new unsigned int[numStudents];

    studentAccountLocks = new uCondition[numStudents];

    for (unsigned int id = 0; id < numStudents; id += 1)
    {
        studentAccountBalances[id] = 0;
    }
}

Bank::~Bank()
{
    delete[] studentAccountBalances;

    delete[] studentAccountLocks;
}

void Bank::deposit(unsigned int id, unsigned int amount)
{
    studentAccountBalances[id] += amount;

    studentAccountLocks[id].signal();
}

void Bank::withdraw(unsigned int id, unsigned int amount)
{
    while (studentAccountBalances[id] < amount)
    {
        studentAccountLocks[id].wait();
    }

    studentAccountBalances[id] -= amount;
}