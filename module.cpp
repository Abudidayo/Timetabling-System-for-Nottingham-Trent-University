#include "Module.h"
#include <iostream>
using namespace std;

// Constructor
Module::Module(string name, int code) : name(name), code(code) {}

// Method to print module details
void Module::printDetails() const {  // Marked as const
    cout << "Module Name: " << name << ", Code: " << code << endl;
}