#include "Module.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

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

// Saves modules to a file.
void saveModulesToFile(const string& filename, const unordered_map<string, Module>& modules) {
    ofstream file(filename);
    for (auto& kv : modules) {
        file << kv.second.getName() << "\n";
    }
}

// Loads modules from a file.
unordered_map<string, Module> loadModulesFromFile(const string& filename) {
    unordered_map<string, Module> modules;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        modules[line] = Module(line);
    }
    return modules;
}

// Adds a module and saves it.
void addModule(unordered_map<string, Module>& modules, const string& filename) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter module name: ";
    string name; 
    getline(cin, name);
    modules[name] = Module(name);
    cout << "Module \"" << name << "\" added.\n";
    saveModulesToFile(filename, modules);
}

// Lists all modules with details.
void listModules(const unordered_map<string, Module>& modules) {
    if (modules.empty()) {
        cout << "No modules available.\n";
        return;
    }
    cout << "\n=== List of Modules ===\n";
    for (auto& kv : modules) {
        kv.second.printDetails();
    }
}

// Manages modules with options to add, delete, or view them.
void manageModules(unordered_map<string, Module>& modules, const string& filename) {
    int choice;
    while (true) {
        cout << "\n--- Manage Modules ---\n"
             << "1. Add Module\n"
             << "2. Delete Module\n"
             << "3. View Modules\n"
             << "4. Back to Admin Menu\n"
             << "Choose an option: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }
        switch (choice) {
            case 1:
                addModule(modules, filename);
                break;
            case 2: {
                if (modules.empty()) {
                    cout << "No modules available to delete.\n";
                    break;
                }
                cout << "\nAvailable Modules:\n";
                vector<string> moduleNames;
                int idx = 1;
                for (const auto& kv : modules) {
                    cout << "  " << idx << ". " << kv.second.getName() << "\n";
                    moduleNames.push_back(kv.first);
                    ++idx;
                }
                cout << "Select a module to delete by number: ";
                int moduleChoice;
                cin >> moduleChoice;
                if (moduleChoice < 1 || moduleChoice > (int)moduleNames.size()) {
                    cout << "Invalid selection.\n";
                    break;
                }
                string moduleName = moduleNames[moduleChoice - 1];
                modules.erase(moduleName);
                saveModulesToFile(filename, modules);
                cout << "Module \"" << moduleName << "\" deleted.\n";
                break;
            }
            case 3:
                listModules(modules);
                break;
            case 4:
                return;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}
