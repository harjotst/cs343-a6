#include "WATCardOffice.h"
#include "Groupoff.h"
#include "Parent.h"
#include "Bank.h"


int main() {
    unsigned int numStudents = 10;
    unsigned int parentalDelay = 2;
    unsigned int numCouriers = 5;
    unsigned int sodaCost = 3;
    unsigned int groupoffDelay = 2;

    Printer printer = Printer();
    Bank bank = Bank(numStudents);
    Parent par = Parent(printer, bank, numStudents, parentalDelay);
    WATCardOffice* office = new WATCardOffice(printer, bank, numCouriers);
    //Groupoff* groupoff = new Groupoff(printer, numStudents, sodaCost, groupoffDelay);


    for(int id = 0; id < numStudents; id++) {
        cout << "call giftcard from main" << endl; // no gift card signals tilI run all at same time
        //WATCard::FWATCard fgc = groupoff->giftCard();

        // Creates a WAT card from WATCard office with balance of $5
        cout << "create fwc" << endl;
		WATCard::FWATCard fwc = office->create(id, 5);
		cout << "returned fwat card" << endl;
        WATCard *card;
        bool lost = false;

		//do { // dont handle lost watcards yet "/"
        try {
            //_Select(fgc)
            //{
            //    card = fgc();
            //    fgc.reset();
            //}
            _Select(fwc)
            {
                
                card = fwc();            
            }    
        }
        _Catch(WATCardOffice::Lost &e)
        {
            cout << "student " << id << "lost watcard" << endl;
            lost = true;
            fwc = office->create(id, 5);
            card = fwc();
            delete card;            

        }
		//} while(lost)

        cout << "student " << id << "WATCard created" << endl;

        delete card;
    }
}