#include "Module.h"
#include <iostream>

// Default constructor implementation
Module::Module() : moduleName("") {}

// Constructor with parameters implementation
Module::Module(string name) : moduleName(name) {}

// Method to print module details
void Module::printDetails() const {
    cout << "Module Name: " << moduleName << endl;
}

// Getter for module name
string Module::getName() const {
    return moduleName;
}

// Setter for module name
void Module::setName(const string& name) {
    moduleName = name;
}
