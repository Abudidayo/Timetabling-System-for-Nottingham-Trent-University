#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
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
    string group; // new field for student group
};

// Load users from file
unordered_map<string, User> loadUsersFromFile(const string& filename) {
    unordered_map<string, User> users;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string u, p, r, g;
        getline(ss, u, ',');
        getline(ss, p, ',');
        getline(ss, r, ',');
        // if group info exists
        if (!getline(ss, g, ',')) {
            g = "";
        }
        users[u] = User{ u, p, r, g };
    }
    return users;
}

// Append a new user to file
void saveUserToFile(const string& filename, const User& user) {
    ofstream file(filename, ios::app);
    file << user.username << "," << user.password << "," << user.role << "," << user.group << "\n";
}

// New function to rewrite entire users file with updated group info
void updateAllUsersFile(const string& filename, const unordered_map<string, User>& users) {
    ofstream file(filename);
    for (const auto& kv : users) {
        file << kv.second.username << "," << kv.second.password << ","
             << kv.second.role << "," << kv.second.group << "\n";
    }
}

// New function to load student groups from student_groups.txt
vector<string> loadStudentGroupsFromFile(const string& filename) {
    vector<string> groups;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            groups.push_back(line);
    }
    return groups;
}

// Modify appendStudentGroup to check for duplicates
void appendStudentGroup(const string& filename, const string& groupName) {
    vector<string> groups = loadStudentGroupsFromFile(filename);
    if (find(groups.begin(), groups.end(), groupName) != groups.end()) {
        cout << "Group \"" << groupName << "\" already exists.\n";
        return;
    }
    ofstream file(filename, ios::app);
    file << groupName << "\n";
    cout << "Group \"" << groupName << "\" added.\n";
}

// Update deleteStudentGroup to display groups and allow selection by number
void deleteStudentGroup(const string& filename) {
    vector<string> groups = loadStudentGroupsFromFile(filename);
    if (groups.empty()) {
        cout << "No groups available to delete.\n";
        return;
    }

    // Display all groups
    cout << "\nAvailable Groups:\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        cout << "  " << (i + 1) << ". " << groups[i] << "\n";
    }

    // Prompt user to select a group by number
    cout << "Select a group to delete by number: ";
    int groupChoice;
    cin >> groupChoice;

    // Validate selection
    if (groupChoice < 1 || groupChoice > (int)groups.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    // Delete the selected group
    string groupName = groups[groupChoice - 1];
    groups.erase(groups.begin() + (groupChoice - 1));
    ofstream file(filename);
    for (const string& group : groups) {
        file << group << "\n";
    }
    cout << "Group \"" << groupName << "\" deleted.\n";
}

// Function to view student groups
void viewStudentGroups(const string& filename) {
    vector<string> groups = loadStudentGroupsFromFile(filename);
    if (groups.empty()) {
        cout << "No student groups available.\n";
        return;
    }

    cout << "\n=== Student Groups ===\n";
    for (const string& group : groups) {
        cout << "- " << group << "\n";
    }
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
    User u{ username, password, "student", "" };
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

// Save modules to file
void saveModulesToFile(const string& filename, const unordered_map<int, Module>& modules) {
    ofstream file(filename);
    for (auto& kv : modules) {
        file << kv.second.getName() << "," << kv.first << "\n";
    }
}

// Load modules from file
unordered_map<int, Module> loadModulesFromFile(const string& filename) {
    unordered_map<int, Module> modules;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        int code;
        getline(ss, name, ',');
        ss >> code;
        modules[code] = Module(name, code);
    }
    return modules;
}

// Save rooms to file
void saveRoomsToFile(const string& filename, const vector<string>& rooms) {
    ofstream file(filename);
    for (const string& room : rooms) {
        file << room << "\n";
    }
}

// Load rooms from file
vector<string> loadRoomsFromFile(const string& filename) {
    vector<string> rooms;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        rooms.push_back(line);
    }
    return rooms;
}

// Show admin menu
void showAdminMenu() {
    cout << "\n=== Admin Menu ===\n"
         << "1. Create Session\n"
         << "2. Manage Groups\n"
         << "3. Manage Modules\n"
         << "4. View Timetable\n"
         << "5. Add Room\n"
         << "6. Exit\n" // Updated exit option
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
void addModule(unordered_map<int, Module>& modules, const string& filename) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter module name: ";
    string name; getline(cin, name);

    cout << "Enter module code: ";
    int code; cin >> code;

    modules[code] = Module(name, code);
    cout << "Module \"" << name << "\" (Code: " << code << ") added.\n";

    saveModulesToFile(filename, modules);
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
    for (int i = 0; i < (int)rooms.size(); ++i) {
        cout << "  " << (i + 1) << ". " << rooms[i] << "\n";
    }
    cout << "Select a room by number: ";
    int roomChoice; cin >> roomChoice;
    if (roomChoice < 1 || roomChoice >(int)rooms.size()) {
        cout << "Invalid room selection.\n";
        return;
    }
    string selectedRoom = rooms[roomChoice - 1];

    // Day-of-week selection
    static const char* days[] = {
        "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"
    };
    cout << "\nSelect day:\n";
    for (int i = 0; i < 7; ++i) {
        cout << "  " << (i + 1) << ": " << days[i] << "\n";
    }
    cout << "Enter choice (1-7): ";
    int dayChoice; cin >> dayChoice;
    if (dayChoice < 1 || dayChoice > 7) {
        cout << "Invalid day selection.\n";
        return;
    }
    string day = days[dayChoice - 1];

    // Time and lecturer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter time of the session: ";
    string time; getline(cin, time);
    cout << "Enter lecturer name: ";
    string lecturer; getline(cin, lecturer);

    // Session type
    cout << "Enter session type (Lecture/Lab): ";
    string type; getline(cin, type);

    // Load and display student groups
    const string groupsFile = "student_groups.txt";
    vector<string> groups = loadStudentGroupsFromFile(groupsFile);
    if (groups.empty()) {
        cout << "No student groups available. Create a group first.\n";
        return;
    }
    cout << "\nAvailable Student Groups:\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        cout << "  " << (i + 1) << ". " << groups[i] << "\n";
    }
    cout << "Select a group by number: ";
    int groupChoice; cin >> groupChoice;
    if (groupChoice < 1 || groupChoice > (int)groups.size()) {
        cout << "Invalid group selection.\n";
        return;
    }
    string selectedGroup = groups[groupChoice - 1];

    // Store and save session
    Session s(type, day, time, selectedRoom, lecturer, moduleCode);
    // Associate the session with the selected group (if needed, extend Session class to include group)
    sessions[moduleCode].push_back(s);
    saveSessionToFile(sessionFile, s);

    cout << "Session created for module code " << moduleCode
        << " on " << day << " at " << time
        << " in room " << selectedRoom
        << ", lecturer " << lecturer
        << ", for group " << selectedGroup << ".\n";
}

// Add a room to the list
void addRoom(vector<string>& rooms) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter room name: ";
    string roomName; getline(cin, roomName);
    rooms.push_back(roomName);
    saveRoomsToFile("rooms.txt", rooms);
    cout << "Room \"" << roomName << "\" added.\n";
}

// View all sessions
void viewAllSessions(const unordered_map<int, vector<Session>>& sessions) {
    if (sessions.empty()) {
        cout << "No sessions available.\n";
        return;
    }
    cout << "\n=== All Sessions ===\n";
    for (auto& kv : sessions) {
        for (auto& sess : kv.second) {
            sess.printSessionDetails();
        }
    }
}

// Update manageGroups to include "View Student Groups"
void manageGroups(unordered_map<string, User>& users) {
    const string groupsFile = "student_groups.txt";
    int choice;
    while (true) {
        cout << "\n--- Manage Groups ---\n"
             << "1. Create New Student Group\n"
             << "2. Assign Student to Group\n"
             << "3. Delete Student Group\n"
             << "4. View Student Groups\n" // New option
             << "5. Back to Admin Menu\n"
             << "Choose an option: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (choice == 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter new group name: ";
            string newGroup; getline(cin, newGroup);
            appendStudentGroup(groupsFile, newGroup);

        } else if (choice == 2) {
            // Filter student users
            vector<string> studentUsernames;
            for (const auto& kv : users) {
                if (kv.second.role == "student")
                    studentUsernames.push_back(kv.second.username);
            }
            if (studentUsernames.empty()) {
                cout << "No student users available.\n";
                continue;
            }
            cout << "\nStudents:\n";
            for (size_t i = 0; i < studentUsernames.size(); ++i)
                cout << "  " << (i + 1) << ". " << studentUsernames[i] << "\n";
            cout << "Select a student by number: ";
            int studentChoice; cin >> studentChoice;
            if (studentChoice < 1 || studentChoice > (int)studentUsernames.size()) {
                cout << "Invalid selection.\n";
                continue;
            }

            // Load groups available
            vector<string> groups = loadStudentGroupsFromFile(groupsFile);
            if (groups.empty()) {
                cout << "No groups available. Create a new group first.\n";
                continue;
            }
            cout << "\nAvailable Groups:\n";
            for (size_t i = 0; i < groups.size(); ++i)
                cout << "  " << (i + 1) << ". " << groups[i] << "\n";
            cout << "Select a group by number: ";
            int groupChoice; cin >> groupChoice;
            if (groupChoice < 1 || groupChoice > (int)groups.size()) {
                cout << "Invalid group selection.\n";
                continue;
            }

            string chosenUser = studentUsernames[studentChoice - 1];
            users[chosenUser].group = groups[groupChoice - 1];
            updateAllUsersFile("users.txt", users);
            cout << "Student \"" << chosenUser << "\" assigned to group \"" << groups[groupChoice - 1] << "\".\n";

        } else if (choice == 3) {
            deleteStudentGroup(groupsFile);

        } else if (choice == 4) { // New case for viewing student groups
            viewStudentGroups(groupsFile);

        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
}

// Update manageModules to display modules and allow selection by number for deletion
void manageModules(unordered_map<int, Module>& modules, const string& filename) {
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

                // Display all modules
                cout << "\nAvailable Modules:\n";
                vector<int> moduleCodes;
                int idx = 1;
                for (const auto& kv : modules) {
                    cout << "  " << idx << ". " << kv.second.getName()
                         << " (Code: " << kv.first << ")\n";
                    moduleCodes.push_back(kv.first);
                    ++idx;
                }

                // Prompt user to select a module by number
                cout << "Select a module to delete by number: ";
                int moduleChoice;
                cin >> moduleChoice;

                // Validate selection
                if (moduleChoice < 1 || moduleChoice > (int)moduleCodes.size()) {
                    cout << "Invalid selection.\n";
                    break;
                }

                // Delete the selected module
                int moduleCode = moduleCodes[moduleChoice - 1];
                string moduleName = modules[moduleCode].getName();
                modules.erase(moduleCode);
                saveModulesToFile(filename, modules);
                cout << "Module \"" << moduleName << "\" (Code: " << moduleCode << ") deleted.\n";
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

int main() {
    const string userFile = "users.txt";
    const string modulesFile = "modules.txt";

    auto users = loadUsersFromFile(userFile);
    auto modules = loadModulesFromFile(modulesFile);
    auto sessions = loadSessionsFromFile(sessionFile);
    auto rooms = loadRoomsFromFile("rooms.txt");

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
            cout << "New username: ";
            cin >> username;
            cout << "New password: ";
            cin >> password;
            if (!registerUser(users, userFile, username, password)) continue;
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
                if (choice == 6) { // Updated exit option
                    cout << "Signing out...\n";
                    break;
                }
                switch (choice) {
                    case 1:
                        createSession(modules, sessions, rooms);
                        break;
                    case 2:
                        manageGroups(users);
                        break;
                    case 3:
                        manageModules(modules, modulesFile);
                        break;
                    case 4:
                        viewAllSessions(sessions);
                        break;
                    case 5:
                        addRoom(rooms);
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
                        cout << "Viewing Timetable...\n";
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

    // Save modules and rooms on exit
    saveModulesToFile(modulesFile, modules);
    saveRoomsToFile("rooms.txt", rooms);
    return 0;
}
