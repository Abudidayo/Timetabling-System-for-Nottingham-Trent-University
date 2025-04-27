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
#include "Timetable.h"

using namespace std;

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

// Updated student menu display without Search and Export options.
void showStudentMenu() {
    cout << "\n=== Student Menu ===\n"
         << "1. View Timetable by Week\n"
         << "2. Exit\n"
         << "Choose an option: ";
}

int main() {
    const string userFile = "users.txt";
    const string modulesFile = "modules.txt";
    const std::string sessionFile = "sessions.txt";

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
                if (choice == 2) {
                    cout << "Signing out...\n";
                    break;
                }
                switch (choice) {
                    case 1:
                        viewStudentTimetable(sessions, users[username].group);
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
