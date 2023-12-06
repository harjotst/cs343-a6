#ifndef TRUCK_H
#define TRUCK_H

_Monitor Printer;

_Task NameServer;

_Task BottlingPlant;

_Task Truck
{
private:
    Printer & printer;

    NameServer & nameServer;

    BottlingPlant & bottlingPlant;

    unsigned int numOfVendingMachines, maxStockPerFlavour;

    /* Checks if cargo is empty. */
    bool isCargoEmpty(unsigned int cargo[4]);

    /* Restocks inventory with cargo.  */
    void restock(unsigned int vid, unsigned int *inventory, unsigned int cargo[4]);

    void main();

public:
    Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
          unsigned int numVendingMachines, unsigned int maxStockPerFlavour); // done

    ~Truck();
};

#endif // TRUCK_H
