#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <limits>
#include "Module.h"
#include "Session.h"

using namespace std;

// Struct to represent a user
struct User {
    string username;
    string password;
    string role; // "admin" or "student"
};

// Load users from file
unordered_map<string, User> loadUsersFromFile(const string& filename) {
    unordered_map<string, User> users;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string u, p, r;
        getline(ss, u, ',');
        getline(ss, p, ',');
        getline(ss, r);
        users[u] = User{ u, p, r };
    }
    return users;
}

// Append a new user to file
void saveUserToFile(const string& filename, const User& user) {
    ofstream file(filename, ios::app);
    file << user.username << "," << user.password << "," << user.role << "\n";
}

// Authenticate credentials
bool authenticateUser(const unordered_map<string, User>& users,
    const string& username,
    const string& password,
    string& outRole) {
    auto it = users.find(username);
    if (it != users.end() && it->second.password == password) {
        outRole = it->second.role;
        return true;
    }
    return false;
}

// Register a new student
bool registerUser(unordered_map<string, User>& users,
    const string& filename,
    const string& username,
    const string& password) {
    if (users.count(username)) {
        cout << "Username already exists.\n";
        return false;
    }
    User u{ username, password, "student" };
    users[username] = u;
    saveUserToFile(filename, u);
    return true;
}

// Session file name
const string sessionFile = "sessions.txt";

// Save a session to file
void saveSessionToFile(const string& filename, const Session& s) {
    ofstream file(filename, ios::app);
    file << s.getSessionType() << ","
        << s.getDay() << ","
        << s.getTime() << ","
        << s.getRoom() << ","
        << s.getLecturer() << ","
        << s.getModuleCode() << "\n";
}

// Load sessions from file
unordered_map<int, vector<Session>> loadSessionsFromFile(const string& filename) {
    unordered_map<int, vector<Session>> sessions;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string type, day, time, room, lecturer;
        int moduleCode;
        getline(ss, type, ',');
        getline(ss, day, ',');
        getline(ss, time, ',');
        getline(ss, room, ',');
        getline(ss, lecturer, ',');
        ss >> moduleCode;
        sessions[moduleCode].emplace_back(
            type, day, time, room, lecturer, moduleCode
        );
    }
    return sessions;
}

// Show admin menu
void showAdminMenu() {
    cout << "\n=== Admin Menu ===\n"
        << "1. Add Module\n"
        << "2. Create Session\n"
        << "3. Manage Groups\n"
        << "4. View Modules\n"
        << "5. View All Sessions\n"
        << "6. Add Room\n"
        << "7. Exit\n"
        << "Choose an option: ";
}

// Show student menu
void showStudentMenu() {
    cout << "\n=== Student Menu ===\n"
        << "1. View Timetable\n"
        << "2. Search Timetable\n"
        << "3. Export Timetable\n"
        << "4. Exit\n"
        << "Choose an option: ";
}

// Add a module
void addModule(unordered_map<int, Module>& modules) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter module name: ";
    string name; getline(cin, name);
    cout << "Enter module code: ";
    int code; cin >> code;
    modules[code] = Module(name, code);
    cout << "Module \"" << name << "\" (Code: " << code << ") added.\n";
}

// List all modules
void listModules(const unordered_map<int, Module>& modules) {
    if (modules.empty()) {
        cout << "No modules available.\n";
        return;
    }
    cout << "\n=== List of Modules ===\n";
    for (auto& kv : modules) {
        kv.second.printDetails();
    }
}

// Create session by picking an existing module
void createSession(const unordered_map<int, Module>& modules,
    unordered_map<int, vector<Session>>& sessions,
    vector<string>& rooms) {
    if (modules.empty()) {
        cout << "No modules available. Add a module first.\n";
        return;
    }

    // Display modules
    vector<int> codes;
    cout << "\nAvailable Modules:\n";
    int idx = 1;
    for (auto& kv : modules) {
        cout << "  " << idx << ". "
            << kv.second.getName()
            << " (Code: " << kv.first << ")\n";
        codes.push_back(kv.first);
        ++idx;
    }

    // Select by index
    cout << "Select a module by number: ";
    int choice; cin >> choice;
    if (cin.fail() || choice < 1 || choice >(int)codes.size()) {
        cout << "Invalid selection.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    int moduleCode = codes[choice - 1];

    // Display rooms
    cout << "\nAvailable Rooms:\n";
    int roomIdx = 1;
    for (const string& room : rooms) {
        cout << "  " << roomIdx << ". " << room << "\n";
        roomIdx++;
    }

    cout << "Select a room by number: ";
    int roomChoice; cin >> roomChoice;
    if (roomChoice < 1 || roomChoice > rooms.size()) {
        cout << "Invalid room selection.\n";
        return;
    }

    string selectedRoom = rooms[roomChoice - 1];

    // Gather session details
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter session type (Lecture/Lab): ";
    string type; getline(cin, type);
    cout << "Enter day of the session: ";
    string day; getline(cin, day);
    cout << "Enter time of the session: ";
    string time; getline(cin, time);
    cout << "Enter lecturer name: ";
    string lecturer; getline(cin, lecturer);

    // Store and save session
    Session s(type, day, time, selectedRoom, lecturer, moduleCode);
    sessions[moduleCode].push_back(s);
    saveSessionToFile(sessionFile, s);

    cout << "Session created for module code " << moduleCode << " in room " << selectedRoom << ".\n";
}

// Add a room to the list
void addRoom(vector<string>& rooms) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter room name: ";
    string roomName;
    getline(cin, roomName);
    rooms.push_back(roomName);
    cout << "Room \"" << roomName << "\" added.\n";
}

// View all sessions
void viewAllSessions(const unordered_map<int, vector<Session>>& sessions) {
    if (sessions.empty()) {
        cout << "No sessions available.\n";
        return;
    }

    cout << "\n=== All Sessions ===\n";
    for (const auto& kv : sessions) {
        for (const auto& session : kv.second) {
            session.printSessionDetails();
        }
    }
}

int main() {
    const string userFile = "users.txt";
    auto users = loadUsersFromFile(userFile);

    unordered_map<int, Module> modules;
    auto sessions = loadSessionsFromFile(sessionFile);
    vector<string> rooms;  // List of rooms

    int choice;
    while (true) {
        // Login/Register
        while (true) {
            cout << "Welcome to the Timetabling System\n"
                << "1. Login\n"
                << "2. Register as Student\n"
                << "Choose an option (1 or 2): ";
            cin >> choice;
            if (!cin.fail() && (choice == 1 || choice == 2)) break;
            cout << "Invalid input. Please enter 1 or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        string username, password, role;
        if (choice == 2) {
            cout << "New username: "; cin >> username;
            cout << "New password: "; cin >> password;
            if (!registerUser(users, userFile, username, password)) continue;
            cout << "Registration successful.\n";
        }

        const int MAX_ATTEMPTS = 3;
        int attempts = 0;
        bool ok = false;
        while (!ok && attempts < MAX_ATTEMPTS) {
            cout << "Username: "; cin >> username;
            cout << "Password: "; cin >> password;
            if (authenticateUser(users, username, password, role)) {
                ok = true;
            }
            else {
                cout << "Invalid credentials. "
                    << (MAX_ATTEMPTS - ++attempts) << " attempts left.\n";
            }
        }
        if (!ok) {
            cout << "Too many failed attempts. Returning to main menu.\n";
            continue;
        }
        cout << "Login successful! Role: " << role << "\n";

        // Role-specific loop
        if (role == "admin") {
            while (true) {
                showAdminMenu();
                cin >> choice;
                if (choice == 7) {
                    cout << "Signing out...\n";
                    break;  // back to login/register
                }
                switch (choice) {
                case 1: addModule(modules); break;
                case 2: createSession(modules, sessions, rooms); break;
                case 3: cout << "Managing Groups...\n"; break;
                case 4: listModules(modules); break;
                case 5: viewAllSessions(sessions); break;
                case 6: addRoom(rooms); break;
                default: cout << "Invalid option.\n";
                }
            }
        }
        else {
            while (true) {
                showStudentMenu();
                cin >> choice;
                if (choice == 4) {
                    cout << "Signing out...\n";
                    break;
                }
                switch (choice) {
                case 1: cout << "Viewing Timetable...\n"; break;
                case 2: cout << "Searching Timetable...\n"; break;
                case 3: cout << "Exporting Timetable...\n"; break;
                default: cout << "Invalid option.\n";
                }
            }
        }
    }

    return 0;
}
