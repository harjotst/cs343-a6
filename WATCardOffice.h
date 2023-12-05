#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H

#include <vector>
#include <deque>

#include "WATCard.h"

_Monitor Printer;

_Monitor Bank;

using namespace std;

_Task WATCardOffice
{
	struct Args
	{
		WATCard *card;

		WATCard::FWATCard fwc;

		unsigned int amount, sid;
	};

	struct Job
	{
		Args args;

		WATCard::FWATCard result;

		Job(Args args) : args(args)
		{
			result = args.fwc;
		};
	};

	Printer & printer;

	Bank & bank;

	unsigned int numCouriers;
	bool closePool;

	deque<Job *> job_buf;

	vector<WATCard *> deliveredWATCards;

	_Task Courier
	{
	private:
		Printer & printer;

		WATCardOffice & office;

		unsigned int id;

		void main();

	public:
		Courier(Printer & printer, WATCardOffice & office, unsigned int id);
	};

	vector<Courier *> couriers;

	void main();

public:
	_Event Lost{};

	WATCardOffice(Printer & prt, Bank & bank, unsigned int numCouriers);

	WATCard::FWATCard create(unsigned int sid, unsigned int amount) __attribute__((warn_unused_result));

	WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard *card) __attribute__((warn_unused_result));

	Job *requestWork() __attribute__((warn_unused_result));
};

#endif // WATCARDOFFICE_H