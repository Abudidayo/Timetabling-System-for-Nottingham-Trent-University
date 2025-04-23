#include "Module.h"
#include <iostream>

// Default constructor implementation
Module::Module() : moduleName(""), moduleCode(0) {}

// Constructor with parameters implementation
Module::Module(string name, int code) : moduleName(name), moduleCode(code) {}

// Method to print module details
void Module::printDetails() const {
    cout << "Module Name: " << moduleName << ", Code: " << moduleCode << endl;
}

// Getter for module name
string Module::getName() const {
    return moduleName;
}

// Getter for module code
int Module::getCode() const {
    return moduleCode;
}

// Setter for module name
void Module::setName(const string& name) {
    moduleName = name;
}

// Setter for module code
void Module::setCode(int code) {
    moduleCode = code;
}
