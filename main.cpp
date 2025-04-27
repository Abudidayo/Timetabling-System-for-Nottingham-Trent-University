#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
#include "Module.h"
#include "Session.h"
#include "User.h"
#include "session.h"

using namespace std;

const std::string sessionFile = "sessions.txt";

// Removed student group functions from main.cpp as they are now in User.cpp.

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

// Saves list of rooms to a file.
void saveRoomsToFile(const string& filename, const vector<string>& rooms) {
    ofstream file(filename);
    for (const string& room : rooms) {
        file << room << "\n";
    }
}

// Loads rooms from a file.
vector<string> loadRoomsFromFile(const string& filename) {
    vector<string> rooms;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        rooms.push_back(line);
    }
    return rooms;
}

// Displays the admin menu.
void showAdminMenu() {
    cout << "\n=== Admin Menu ===\n"
         << "1. Manage Sessions\n"
         << "2. Manage Groups\n"
         << "3. Manage Modules\n"
         << "4. Manage Rooms\n"
         << "5. View All Sessions\n"
         << "6. Exit\n"
         << "Choose an option: ";
}

// Displays the student menu.
void showStudentMenu() {
    cout << "\n=== Student Menu ===\n"
         << "1. View Timetable by Week\n"
         << "2. Search Timetable\n"
         << "3. Export Timetable\n"
         << "4. Exit\n"
         << "Choose an option: ";
}

// Adds a module and saves it.
void addModule(unordered_map<string, Module>& modules, const string& filename) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter module name: ";
    string name; getline(cin, name);
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

// Adds a room and saves it.
void addRoom(vector<string>& rooms) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter room name: ";
    string roomName; getline(cin, roomName);
    rooms.push_back(roomName);
    saveRoomsToFile("rooms.txt", rooms);
    cout << "Room \"" << roomName << "\" added.\n";
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

// Manages rooms with options to add, delete, or view them.
void manageRooms(vector<string>& rooms) {
    int choice;
    while (true) {
        cout << "\n--- Manage Rooms ---\n"
             << "1. Add Room\n"
             << "2. Delete Room\n"
             << "3. View All Rooms\n"
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
            case 1: {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter room name: ";
                string roomName; getline(cin, roomName);
                rooms.push_back(roomName);
                saveRoomsToFile("rooms.txt", rooms);
                cout << "Room \"" << roomName << "\" added.\n";
                break;
            }
            case 2: {
                if (rooms.empty()) {
                    cout << "No rooms available to delete.\n";
                    break;
                }
                cout << "\nAvailable Rooms:\n";
                for (size_t i = 0; i < rooms.size(); ++i) {
                    cout << "  " << (i + 1) << ". " << rooms[i] << "\n";
                }
                cout << "Select a room to delete by number: ";
                int roomChoice;
                cin >> roomChoice;
                if (roomChoice < 1 || roomChoice > (int)rooms.size()) {
                    cout << "Invalid selection.\n";
                    break;
                }
                string roomName = rooms[roomChoice - 1];
                rooms.erase(rooms.begin() + (roomChoice - 1));
                saveRoomsToFile("rooms.txt", rooms);
                cout << "Room \"" << roomName << "\" deleted.\n";
                break;
            }
            case 3: {
                if (rooms.empty()) {
                    cout << "No rooms available.\n";
                    break;
                }
                cout << "\n=== List of Rooms ===\n";
                for (const string& room : rooms) {
                    cout << "- " << room << "\n";
                }
                break;
            }
            case 4:
                return;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}

// Views the student's timetable for a specified week.
void viewStudentTimetable(const unordered_map<string, vector<Session>>& sessions, const string& studentGroup) {
    cout << "Enter week number to view timetable: ";
    int week;
    cin >> week;
    if (cin.fail() || week < 1) {
        cout << "Invalid week number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cout << "\n=== Timetable for Week " << week << " ===\n";
    bool found = false;
    for (const auto& kv : sessions) {
        for (const auto& s : kv.second) {
            if (s.getStartingWeek() <= week && s.getEndingWeek() >= week && s.getStudentGroup() == studentGroup) {
                cout << s.getDay() << " | " << s.getSessionType() << " from " << s.getTime()
                     << " in " << s.getRoom() << " with " << s.getLecturer() << "\n";
                found = true;
            }
        }
    }
    if (!found)
        cout << "No sessions available for week " << week << " in your group.\n";
}

int main() {
    const string userFile = "users.txt";
    const string modulesFile = "modules.txt";

    auto users = loadUsersFromFile(userFile);
    auto modules = loadModulesFromFile(modulesFile);
    auto sessions = loadSessionsFromFile(sessionFile);
    auto rooms = loadRoomsFromFile("rooms.txt");

    int choice;
    while (true) {
        cout << "Welcome to the Timetabling System\n"
             << "1. Login\n"
             << "2. Register as Student\n"
             << "Choose an option (1 or 2): ";
        cin >> choice;
        if (!cin.fail() && (choice == 1 || choice == 2)) { }
        else {
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        string username, password, role;
        if (choice == 2) {
            cout << "New username: ";
            cin >> username;
            cout << "New password: ";
            cin >> password;
            if (!registerUser(users, userFile, username, password))
                continue;
            cout << "Registration successful.\n";
        }

        const int MAX_ATTEMPTS = 3;
        int attempts = 0;
        bool ok = false;
        while (!ok && attempts < MAX_ATTEMPTS) {
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            if (authenticateUser(users, username, password, role)) {
                ok = true;
            } else {
                cout << "Invalid credentials. " << (MAX_ATTEMPTS - ++attempts) << " attempts left.\n";
            }
        }
        if (!ok) {
            cout << "Too many failed attempts. Returning to main menu.\n";
            continue;
        }
        cout << "Login successful! Role: " << role << "\n";

        if (role == "admin") {
            while (true) {
                showAdminMenu();
                cin >> choice;
                if (choice == 6) {
                    cout << "Signing out...\n";
                    break;
                }
                switch (choice) {
                    case 1:
                        manageSessions(modules, sessions, rooms);
                        break;
                    case 2:
                        manageGroups(users);
                        break;
                    case 3:
                        manageModules(modules, modulesFile);
                        break;
                    case 4:
                        manageRooms(rooms);
                        break;
                    case 5:
                        viewAllSessions(sessions);
                        break;
                    default:
                        cout << "Invalid option.\n";
                }
            }
        } else {
            while (true) {
                showStudentMenu();
                cin >> choice;
                if (choice == 4) {
                    cout << "Signing out...\n";
                    break;
                }
                switch (choice) {
                    case 1:
                        viewStudentTimetable(sessions, users[username].group);
                        break;
                    case 2:
                        cout << "Searching Timetable...\n";
                        break;
                    case 3:
                        cout << "Exporting Timetable...\n";
                        break;
                    default:
                        cout << "Invalid option.\n";
                }
            }
        }
    }

    saveModulesToFile(modulesFile, modules);
    saveRoomsToFile("rooms.txt", rooms);
    return 0;
}
