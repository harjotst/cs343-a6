#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <uPRNG.h>

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

string getFileAsStr(const char* configFileName)
{
    try {
        ifstream file(configFileName);

        stringstream buffer;

        buffer << file.rdbuf();

        string contents = buffer.str();

        file.close();

        return contents;
        
    } catch( uFile::Failure & ) {
        cerr << "Error: Could not open input file \"" << configFileName << "\"" << endl;
        exit( EXIT_FAILURE );
    }
    }

int main(int argc, char **argv)
{
    char* configFileName = "soda.config";
    unsigned int processors = 1;

    ifstream file;

	struct cmd_error {};
	
	try {
		switch ( argc ) {
		  case 4:
            if ( strcmp( argv[3], "d" ) != 0 ) {
                processors = convert( argv[3] ); 
                if ( processors <= 0 ) {
                    throw cmd_error();
                }
            }
          case 3:
            if ( strcmp( argv[2], "d" ) != 0 ) {
                unsigned int seed = convert( argv[2] ); 
                if ( seed <= 0 ) {
                    throw cmd_error();
                } else {
                    set_seed(seed);
                }
            }
          case 2:
            if ( strcmp( argv[1], "d" ) != 0 ) {
                configFileName = argv[1];
            }
		}
	} catch( ... ) {
		cerr << "Usage: ./soda [ config-file | 'd' (default file soda.config) [ seed (> 0) | 'd' (default random) [ processors (> 0) | 'd' (default 1) ] ] ]" << endl;
		exit( EXIT_FAILURE );
	}

    uProcessor p[processors - 1];

    ConfigParms config;

    string fileContent = getFileAsStr(configFileName);

    processConfigFile(fileContent.c_str(), config);

    cout << config << endl << endl;

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