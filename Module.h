#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <limits>
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

// Added function declarations moved from main.cpp
void saveModulesToFile(const string& filename, const unordered_map<string, Module>& modules);
unordered_map<string, Module> loadModulesFromFile(const string& filename);
void addModule(unordered_map<string, Module>& modules, const string& filename);
void listModules(const unordered_map<string, Module>& modules);
void manageModules(unordered_map<string, Module>& modules, const string& filename);

#endif  // MODULE_H
