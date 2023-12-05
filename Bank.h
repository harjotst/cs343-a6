#ifndef BANK_H
#define BANK_H

_Monitor Bank
{
	unsigned int *student_accounts;
	
	uCondition *account_locks;

public:
	Bank(unsigned int numStudents);

	~Bank();

	void deposit(unsigned int id, unsigned int amount);

	void withdraw(unsigned int id, unsigned int amount);
};

#endif // BANK_h