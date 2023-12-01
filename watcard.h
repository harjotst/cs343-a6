#ifndef WATCARD_H
#define WATCARD_H

class WATCard
{
private:
    /* prevent copying or moving of WATCard */
    WATCard(const WATCard &) = delete;

    WATCard(const WATCard &&) = delete;

    /* prevent assignments of one WATCard to another */
    WATCard &operator=(const WATCard &) = delete;

    WATCard &operator=(const WATCard &&) = delete;

    /* How much money is currently on WATCard */
    unsigned int balance;

public:
    // future watcard pointer
    typedef Future_ISM<WATCard *> FWATCard;

    WATCard();
    
    void deposit(unsigned int amount); // done
    
    void withdraw(unsigned int amount); // done
    
    unsigned int getBalance(); // done
};

#endif // WATCARD_H
