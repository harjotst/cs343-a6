#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "ConfigParams.h"
#include "Printer.h"
#include "VendingMachine.h"
#include "NameServer.h"
#include "Bank.h"
#include "WATCardOffice.h"
#include "Groupoff.h"
#include "Parent.h"
#include "Student.h"

using namespace std;

string getFileAsStr(const char *fileName)
{
    ifstream file(fileName);

    if (!file.is_open())
    {
        cerr << "Failed to open file\n";
    }

    stringstream buffer;

    buffer << file.rdbuf();

    string contents = buffer.str();

    file.close();

    return contents;
}

int main(int argc, char **argv)
{
    ConfigParms config;

    string fileContent = getFileAsStr(argv[1]);

    processConfigFile(fileContent.c_str(), config);

    cout << config << endl
         << endl;

    Printer *printer = new Printer(config.numStudents, config.numVendingMachines, config.numCouriers);

    NameServer *nameServer = new NameServer(*printer, config.numVendingMachines, config.numStudents);

    BottlingPlant *bottlingPlant = new BottlingPlant(*printer, *nameServer, config.numVendingMachines,
                                                     config.maxShippedPerFlavour, config.maxStockPerFlavour,
                                                     config.timeBetweenShipments);

    Bank bank(config.numStudents);

    Parent *parent = new Parent(*printer, bank, config.numStudents, config.parentalDelay);

    WATCardOffice *office = new WATCardOffice(*printer, bank, config.numCouriers);

    Groupoff *groupoff = new Groupoff(*printer, config.numStudents, config.sodaCost, config.groupoffDelay);

    VendingMachine **vendingMachines = new VendingMachine *[config.numVendingMachines];

    for (unsigned int id = 0; id < config.numVendingMachines; id += 1)
        vendingMachines[id] = new VendingMachine(*printer, *nameServer, id, config.sodaCost);

    Student **students = new Student *[config.numStudents];

    for (unsigned int id = 0; id < config.numStudents; id += 1)
        students[id] = new Student(*printer, *nameServer, *office, *groupoff, id, config.maxPurchases);

    for (unsigned int id = 0; id < config.numStudents; id += 1)
        delete students[id];

    delete[] students;

    delete bottlingPlant;

    for (unsigned int id = 0; id < config.numVendingMachines; id += 1)
        delete vendingMachines[id];

    delete[] vendingMachines;

    delete office;

    delete groupoff;

    delete parent;

    delete nameServer;

    delete printer;
}