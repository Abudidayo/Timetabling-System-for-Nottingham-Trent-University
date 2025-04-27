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
#include "session.h"  // Include session management declarations

using namespace std;

const std::string sessionFile = "sessions.txt"; // Added sessionFile definition

//
// Function to load student groups from student_groups.txt
//
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

//
// Checks for duplicate group names and appends a new group if not exists.
//
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

//
// Displays groups and allows the user to delete a selected group.
//
void deleteStudentGroup(const string& filename) {
    vector<string> groups = loadStudentGroupsFromFile(filename);
    if (groups.empty()) {
        cout << "No groups available to delete.\n";
        return;
    }
    cout << "\nAvailable Groups:\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        cout << "  " << (i + 1) << ". " << groups[i] << "\n";
    }
    cout << "Select a group to delete by number: ";
    int groupChoice;
    cin >> groupChoice;
    if (groupChoice < 1 || groupChoice > (int)groups.size()) {
        cout << "Invalid selection.\n";
        return;
    }
    string groupName = groups[groupChoice - 1];
    groups.erase(groups.begin() + (groupChoice - 1));
    ofstream file(filename);
    for (const string& group : groups) {
        file << group << "\n";
    }
    cout << "Group \"" << groupName << "\" deleted.\n";
}

//
// Displays the available student groups.
//
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

//
// Saves modules to a file.
//
void saveModulesToFile(const string& filename, const unordered_map<string, Module>& modules) {
    ofstream file(filename);
    for (auto& kv : modules) {
        file << kv.second.getName() << "\n";
    }
}

//
// Loads modules from a file.
//
unordered_map<string, Module> loadModulesFromFile(const string& filename) {
    unordered_map<string, Module> modules;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        modules[line] = Module(line);
    }
    return modules;
}

//
// Saves list of rooms to a file.
//
void saveRoomsToFile(const string& filename, const vector<string>& rooms) {
    ofstream file(filename);
    for (const string& room : rooms) {
        file << room << "\n";
    }
}

//
// Loads rooms from a file.
//
vector<string> loadRoomsFromFile(const string& filename) {
    vector<string> rooms;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        rooms.push_back(line);
    }
    return rooms;
}

//
// Displays the admin menu.
//
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

//
// Displays the student menu.
//
void showStudentMenu() {
    cout << "\n=== Student Menu ===\n"
         << "1. View Timetable by Week\n"
         << "2. Search Timetable\n"
         << "3. Export Timetable\n"
         << "4. Exit\n"
         << "Choose an option: ";
}

//
// Adds a module and saves it.
//
void addModule(unordered_map<string, Module>& modules, const string& filename) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter module name: ";
    string name; getline(cin, name);
    modules[name] = Module(name);
    cout << "Module \"" << name << "\" added.\n";
    saveModulesToFile(filename, modules);
}

//
// Lists all modules with details.
//
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

//
// Adds a room and saves it.
//
void addRoom(vector<string>& rooms) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter room name: ";
    string roomName; getline(cin, roomName);
    rooms.push_back(roomName);
    saveRoomsToFile("rooms.txt", rooms);
    cout << "Room \"" << roomName << "\" added.\n";
}

//
// Manages student groups by providing options to create, assign, delete, or view groups.
//
void manageGroups(unordered_map<string, User>& users) {
    const string groupsFile = "student_groups.txt";
    int choice;
    while (true) {
        cout << "\n--- Manage Groups ---\n"
             << "1. Create New Student Group\n"
             << "2. Assign Student to Group\n"
             << "3. Delete Student Group\n"
             << "4. View Student Groups\n"
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
        } else if (choice == 4) {
            viewStudentGroups(groupsFile);
        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
}

//
// Manages modules with options to add, delete, or view them.
//
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

//
// Manages rooms with options to add, delete, or view them.
//
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

//
// Views the student's timetable for a specified week.
//
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
