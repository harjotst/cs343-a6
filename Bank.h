#ifndef BANK_H
#define BANK_H

_Monitor Bank
{
private:
	/* array for keeping track of account balances of each student. */
	unsigned int *studentAccountBalances;
	
	/* condition locks for students to wait on in the event that
	   there are insufficient funds in their account and to be woken
	   up from once there are sufficient funds. */
	uCondition *studentAccountLocks;

public:
	Bank(unsigned int numStudents);

	~Bank();

	/* called by student "id" to deposit amount into their acccount.
	   may wake thread waiting for sufficient funds in their account
	   to withdraw from. */
	void deposit(unsigned int id, unsigned int amount);

	/* called by student "id" to withdraw amount from their account.
	   may block if there are insufficient funds in their account. */
	void withdraw(unsigned int id, unsigned int amount);
};

#endif // BANK_h