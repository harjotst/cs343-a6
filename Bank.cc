#include "Bank.h"

Bank::Bank(unsigned int numStudents)
{
    studentAccountBalances = new unsigned int[numStudents];

    withdrawAmountRequests = new int[numStudents];

    for (unsigned int id = 0; id < numStudents; id += 1)
    {
        withdrawAmountRequests[id] = -1;
    }

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

    delete[] withdrawAmountRequests;
}

void Bank::deposit(unsigned int id, unsigned int amount)
{
    studentAccountBalances[id] += amount;

    if (withdrawAmountRequests[id] != -1 && studentAccountBalances[id] >= withdrawAmountRequests[id])
    {
       studentAccountLocks[id].signal();
    }
}

void Bank::withdraw(unsigned int id, unsigned int amount)
{
    if (studentAccountBalances[id] < amount)
    {
        withdrawAmountRequests[id] = amount;

        studentAccountLocks[id].wait();
    }

    studentAccountBalances[id] -= amount;

    withdrawAmountRequests[id] = -1;
}
