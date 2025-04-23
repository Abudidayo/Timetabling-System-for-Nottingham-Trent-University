#ifndef MODULE_H
#define MODULE_H

#include <string>
using namespace std;

class Module {
private:
    string name;
    int code;

public:
    // Constructor
    Module(string name, int code);

    // Method to print module details
    void printDetails() const;  // Marked as const
};

#endif  // MODULE_H
#pragma once