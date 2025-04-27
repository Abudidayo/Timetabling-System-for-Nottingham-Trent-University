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

using namespace std;

const std::string sessionFile = "sessions.txt";

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
    // Now using room functions from session.cpp
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
                // Using showAdminMenu and room functions declared in session.h.
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
