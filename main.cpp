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

// Removed the definitions of the following functions:
// - loadUsersFromFile
// - saveUserToFile
// - updateAllUsersFile
// - authenticateUser
// - registerUser

// The rest of the code remains unchanged.

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

// Save modules to file
void saveModulesToFile(const string& filename, const unordered_map<string, Module>& modules) {
    ofstream file(filename);
    for (auto& kv : modules) {
        file << kv.second.getName() << "\n";
    }
}

// Load modules from file
unordered_map<string, Module> loadModulesFromFile(const string& filename) {
    unordered_map<string, Module> modules;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        modules[line] = Module(line);
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
         << "1. Manage Sessions\n"
         << "2. Manage Groups\n"
         << "3. Manage Modules\n"
         << "4. Manage Rooms\n" // Updated order
         << "5. View All Sessions\n" // Moved to after "Manage Rooms"
         << "6. Exit\n"
         << "Choose an option: ";
}

// Show student menu
void showStudentMenu() {
    cout << "\n=== Student Menu ===\n"
        << "1. View Timetable by Week\n" // Updated option
        << "2. Search Timetable\n"
        << "3. Export Timetable\n"
        << "4. Exit\n"
        << "Choose an option: ";
}

// Add a module
void addModule(unordered_map<string, Module>& modules, const string& filename) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter module name: ";
    string name; getline(cin, name);

    modules[name] = Module(name);
    cout << "Module \"" << name << "\" added.\n";

    saveModulesToFile(filename, modules);
}

// List all modules
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

// Add a room to the list
void addRoom(vector<string>& rooms) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter room name: ";
    string roomName; getline(cin, roomName);
    rooms.push_back(roomName);
    saveRoomsToFile("rooms.txt", rooms);
    cout << "Room \"" << roomName << "\" added.\n";
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

                // Display all modules
                cout << "\nAvailable Modules:\n";
                vector<string> moduleNames;
                int idx = 1;
                for (const auto& kv : modules) {
                    cout << "  " << idx << ". " << kv.second.getName() << "\n";
                    moduleNames.push_back(kv.first);
                    ++idx;
                }

                // Prompt user to select a module by number
                cout << "Select a module to delete by number: ";
                int moduleChoice;
                cin >> moduleChoice;

                // Validate selection
                if (moduleChoice < 1 || moduleChoice > (int)moduleNames.size()) {
                    cout << "Invalid selection.\n";
                    break;
                }

                // Delete the selected module
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

// New function to manage rooms
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
            case 1: { // Add Room
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter room name: ";
                string roomName; getline(cin, roomName);
                rooms.push_back(roomName);
                saveRoomsToFile("rooms.txt", rooms);
                cout << "Room \"" << roomName << "\" added.\n";
                break;
            }
            case 2: { // Delete Room
                if (rooms.empty()) {
                    cout << "No rooms available to delete.\n";
                    break;
                }

                // Display all rooms
                cout << "\nAvailable Rooms:\n";
                for (size_t i = 0; i < rooms.size(); ++i) {
                    cout << "  " << (i + 1) << ". " << rooms[i] << "\n";
                }

                // Prompt user to select a room by number
                cout << "Select a room to delete by number: ";
                int roomChoice;
                cin >> roomChoice;

                // Validate selection
                if (roomChoice < 1 || roomChoice > (int)rooms.size()) {
                    cout << "Invalid selection.\n";
                    break;
                }

                // Delete the selected room
                string roomName = rooms[roomChoice - 1];
                rooms.erase(rooms.begin() + (roomChoice - 1));
                saveRoomsToFile("rooms.txt", rooms);
                cout << "Room \"" << roomName << "\" deleted.\n";
                break;
            }
            case 3: { // View All Rooms
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
            case 4: // Back to Admin Menu
                return;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}

// Updated function to view the student's timetable by week number
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

    // Display title for the respective week number
    cout << "\n=== Timetable for Week " << week << " ===\n";

    bool found = false;
    for (const auto& kv : sessions) {
        for (const auto& s : kv.second) {
            if (s.getStartingWeek() <= week && s.getEndingWeek() >= week && s.getStudentGroup() == studentGroup) {
                // Print session details in the specified format
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
    auto sessions = loadSessionsFromFile(sessionFile);  // now sessionFile is defined
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
                if (choice == 6) { // Exit option
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
                        manageRooms(rooms); // Updated: now option 4
                        break;
                    case 5:
                        viewAllSessions(sessions); // Updated: now option 5
                        break;
                    default:
                        cout << "Invalid option.\n";
                }
            }
        } else {  // Student menu
            while (true) {
                showStudentMenu();
                cin >> choice;
                if (choice == 4) {
                    cout << "Signing out...\n";
                    break;
                }
                switch (choice) {
                    case 1:
                        // Use the logged-in student's group
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

    // Save modules and rooms on exit
    saveModulesToFile(modulesFile, modules);
    saveRoomsToFile("rooms.txt", rooms);
    return 0;
}
