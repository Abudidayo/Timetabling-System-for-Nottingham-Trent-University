#ifndef MODULE_H
#define MODULE_H

#include <string>
using namespace std;

class Module {
private:
    string moduleName;
    int moduleCode;

public:
    // Default constructor
    Module();

    // Constructor with parameters
    Module(string name, int code);

    // Method to print module details
    void printDetails() const;

    // Getter methods
    string getName() const;
    int getCode() const;

    // Setter methods
    void setName(const string& name);
    void setCode(int code);
};

#endif  // MODULE_H
