#ifndef CONFIG_PARAMS_H
#define CONFIG_PARAMS_H

#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <cstdlib>

using namespace std;

struct ConfigParms
{
    unsigned int sodaCost;             // MSRP per bottle
    unsigned int numStudents;          // number of students to create
    unsigned int maxPurchases;         // maximum number of bottles a student purchases
    unsigned int numVendingMachines;   // number of vending machines
    unsigned int maxStockPerFlavour;   // maximum number of bottles of each flavour stocked
    unsigned int maxShippedPerFlavour; // number of bottles of each flavour in a shipment
    unsigned int timeBetweenShipments; // length of time between shipment pickup
    unsigned int groupoffDelay;        // length of time between initializing gift cards
    unsigned int parentalDelay;        // length of time between cash deposits
    unsigned int numCouriers;          // number of couriers in the pool

    friend std::ostream &operator<<(ostream &os, const ConfigParms &cp)
    {
        os << "SodaCost: " << cp.sodaCost << '\n'
           << "NumStudents: " << cp.numStudents << '\n'
           << "MaxPurchases: " << cp.maxPurchases << '\n'
           << "NumVendingMachines: " << cp.numVendingMachines << '\n'
           << "MaxStockPerFlavour: " << cp.maxStockPerFlavour << '\n'
           << "MaxShippedPerFlavour: " << cp.maxShippedPerFlavour << '\n'
           << "TimeBetweenShipments: " << cp.timeBetweenShipments << '\n'
           << "GroupoffDelay: " << cp.groupoffDelay << '\n'
           << "ParentalDelay: " << cp.parentalDelay << '\n'
           << "NumCouriers: " << cp.numCouriers;

        return os;
    }
};

/* Helper function to skip whitespace characters. */
const char *skipWhitespace(const char *current)
{
    while (isspace((unsigned char)*current))
    {
        current += 1;
    }

    return current;
}

/* Helper function to read an unsigned integer from the string. */
const char *readUnsignedInt(const char *str, unsigned int &outValue)
{
    outValue = strtoul(str, const_cast<char **>(&str), 10);

    return str;
}

void processConfigFile(const char *configFile, ConfigParms &cparms)
{
    /* Pointer to current character in config file. */
    const char *current = configFile;

    /* Continue until the end of the string. */
    while (*current != '\0')
    {
        /* Skip whitespace before the key. */
        current = skipWhitespace(current);

        /* Check if we're at the end of the string or a comment. */
        if (*current == '\0' || *current == '#')
        {
            while (*current != '\0' && *current != '\n')
            {
                current += 1; /* Skip to the end of the line */
            }

            continue;
        }

        /* Get the key by reading until the next whitespace */
        const char *keyStart = current;

        while (*current != '\0' && !isspace((unsigned char)*current))
        {
            current += 1;
        }

        string key(keyStart, current);

        // Skip whitespace before the value
        current = skipWhitespace(current);

        unsigned int value;

        // Read the value
        current = readUnsignedInt(current, value);

        // Assign the value to the correct field in the config structure
        if (key == "SodaCost")
            cparms.sodaCost = value;
        else if (key == "NumStudents")
            cparms.numStudents = value;
        else if (key == "MaxPurchases")
            cparms.maxPurchases = value;
        else if (key == "NumVendingMachines")
            cparms.numVendingMachines = value;
        else if (key == "MaxStockPerFlavour")
            cparms.maxStockPerFlavour = value;
        else if (key == "MaxShippedPerFlavour")
            cparms.maxShippedPerFlavour = value;
        else if (key == "TimeBetweenShipments")
            cparms.timeBetweenShipments = value;
        else if (key == "GroupoffDelay")
            cparms.groupoffDelay = value;
        else if (key == "ParentalDelay")
            cparms.parentalDelay = value;
        else if (key == "NumCouriers")
            cparms.numCouriers = value;

        // Skip to the end of the line after processing a value
        while (*current != '\0' && *current != '\n')
        {
            current += 1;
        }
    }
}

#endif // CONFIG_PARAMS_H