#include "watcardoffice.h"
#include "groupoff.h"
#include "parent.h"
#include "bank.h"

using namespace std;

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
    Groupoff* groupoff = new Groupoff(printer, numStudents, sodaCost, groupoffDelay);

    WATCard::FWATCard fwc;
    cout << "call get fgc" << endl;
    WATCard::FWATCard fgc = groupoff->giftCard();
    cout << "return from get fgc" << endl;
    
    for(int id = 0; id < numStudents; id++) {
        cout << "call giftcard from main" << endl; // no gift card signals tilI run all at same time
        // Creates a WAT card from WATCard office with balance of $5
        cout << "create fwc" << endl;
		cout << "call get fwc" << endl;
        fwc = office->create(id, 5);
		cout << "returned fwat card" << endl;
        WATCard *card;
        bool lost = false;

        try {
            _Select(fgc)
            {
                cout << "GET GIFT CARD" << endl;
                card = fgc();
                fgc.reset();                
            }
            or _Select(fwc)
            {
                cout << "GET WATCard" << endl;
                card = fwc();  
                cout << "GOT WATCard" << endl;
                
            }    

            
        }
        _Catch(WATCardOffice::Lost &e)
        {
            cout << "student " << id << " LOST WATCARD " << endl;          
        }

        
    }
    cout << "call watcardoffice destructor" << endl;
    delete office;
    cout << "call groupoff destructor" << endl;    
    delete groupoff;
}