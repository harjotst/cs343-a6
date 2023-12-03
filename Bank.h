#pragma once

// manages all the students account info
_Monitor Bank {
	unsigned int *student_accounts;
	uCondition *account_locks;

  public:
	Bank( unsigned int numStudents ) {
		student_accounts = new unsigned int[numStudents]; // maybe use uNoCtor - apparently C++ guarentees mem is 0 initialized does uC++?
		account_locks = new uCondition[numStudents];

		// all students start with $0
		for(int i = 0; i < numStudents; i++) {
			student_accounts[i] = 0;
		}
	}

	// destructor called by main at the end of execution
	~Bank() {
		delete [] student_accounts;
		delete [] account_locks;
	}

	void deposit( unsigned int id, unsigned int amount ) {
		student_accounts[id] += amount;
		account_locks[id].signal();
	}

	void withdraw( unsigned int id, unsigned int amount ) {
		// if the students account doesn't have enough money to withdraw amount, wait until it does
		// this can take more than one deposit
		while(student_accounts[id] < amount) {
			// wait on that specific deposit into this students account
			account_locks[id].wait();
		}

		student_accounts[id] -= amount;
	}

};