#ifndef MODULE_H
#define MODULE_H

#include <string>
using namespace std;

class Module {
private:
    string moduleName;

public:
    // Default constructor
    Module();

    // Constructor with parameters
    Module(string name);

    // Method to print module details
    void printDetails() const;

    // Getter methods
    string getName() const;

    // Setter methods
    void setName(const string& name);
};

#endif  // MODULE_H
