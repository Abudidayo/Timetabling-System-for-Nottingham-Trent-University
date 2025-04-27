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

// Updated function to save a session to file with week numbers
void saveSessionToFile(const string& filename, const Session& s) {
    ofstream file(filename, ios::app);
    file << s.getSessionType() << ","
         << s.getDay() << ","
         << s.getTime() << ","
         << s.getRoom() << ","
         << s.getLecturer() << ","
         << s.getModuleName() << ","
         << s.getStudentGroup() << ","
         << s.getStartingWeek() << ","
         << s.getEndingWeek() << "\n"; // Include week numbers
}

// Updated loadSessionsFromFile to read week numbers
unordered_map<string, vector<Session>> loadSessionsFromFile(const string& filename) {
    unordered_map<string, vector<Session>> sessions;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string type, day, time, room, lecturer, moduleName, studentGroup, startWeekStr, endWeekStr;
        getline(ss, type, ',');
        getline(ss, day, ',');
        getline(ss, time, ',');
        getline(ss, room, ',');
        getline(ss, lecturer, ',');
        getline(ss, moduleName, ',');
        getline(ss, studentGroup, ','); // Read student group
        getline(ss, startWeekStr, ',');   // Read starting week
        getline(ss, endWeekStr, ',');       // Read ending week
        int startWeek = startWeekStr.empty() ? 0 : std::stoi(startWeekStr);
        int endWeek = endWeekStr.empty() ? 0 : std::stoi(endWeekStr);
        sessions[moduleName].emplace_back(type, day, time, room, lecturer, moduleName, studentGroup, startWeek, endWeek);
    }
    return sessions;
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

// Create session by picking an existing module
void createSession(const unordered_map<string, Module>& modules,
                   unordered_map<string, vector<Session>>& sessions,
                   vector<string>& rooms) {
    if (modules.empty()) {
        cout << "No modules available. Add a module first.\n";
        return;
    }

    // Display modules
    vector<string> names;
    cout << "\nAvailable Modules:\n";
    int idx = 1;
    for (auto& kv : modules) {
        cout << "  " << idx << ". " << kv.second.getName() << "\n";
        names.push_back(kv.first);
        ++idx;
    }

    cout << "Select a module by number: ";
    int choice; cin >> choice;
    if (cin.fail() || choice < 1 || choice > (int)names.size()) {
        cout << "Invalid selection.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    string moduleName = names[choice - 1];

    // Display rooms
    cout << "\nAvailable Rooms:\n";
    for (int i = 0; i < (int)rooms.size(); ++i) {
        cout << "  " << (i + 1) << ". " << rooms[i] << "\n";
    }
    cout << "Select a room by number: ";
    int roomChoice; cin >> roomChoice;
    if (roomChoice < 1 || roomChoice > (int)rooms.size()) {
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

    // Prompt for starting and ending week numbers
    cout << "Enter starting week number: ";
    int startingWeek; cin >> startingWeek;
    cout << "Enter ending week number: ";
    int endingWeek; cin >> endingWeek;
    if(cin.fail() || startingWeek < 1 || endingWeek < startingWeek) {
        cout << "Invalid week numbers provided.\n";
        return;
    }

    // Store and save session with week numbers
    Session s(type, day, time, selectedRoom, lecturer, moduleName, selectedGroup, startingWeek, endingWeek);
    sessions[moduleName].push_back(s);
    saveSessionToFile(sessionFile, s);

    cout << "Session created for module \"" << moduleName
         << "\" on " << day << " at " << time
         << " in room " << selectedRoom
         << ", lecturer " << lecturer
         << ", for group " << selectedGroup
         << ", Weeks " << startingWeek << " to " << endingWeek << ".\n";
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
void viewAllSessions(const unordered_map<string, vector<Session>>& sessions) {
    if (sessions.empty()) {
        cout << "No sessions available.\n";
        return;
    }
    cout << "\n=== All Sessions ===\n";
    for (auto& kv : sessions) {
        for (auto& sess : kv.second) {
            sess.printSessionDetails(); // Updated to include student group
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

// Helper function to update sessions file after deletion
void updateSessionsFile(const string& filename, const unordered_map<string, vector<Session>>& sessions) {
    ofstream file(filename);
    // Rewrite the sessions file based on the current sessions map
    for (const auto& kv : sessions) {
        for (const Session& s : kv.second) {
            file << s.getSessionType() << ","
                 << s.getDay() << ","
                 << s.getTime() << ","
                 << s.getRoom() << ","
                 << s.getLecturer() << ","
                 << s.getModuleName() << ","
                 << s.getStudentGroup() << ","
                 << s.getStartingWeek() << ","
                 << s.getEndingWeek() << "\n"; // Include week numbers
        }
    }
}

// New function to manage sessions (create and delete)
void manageSessions(const unordered_map<string, Module>& modules,
                    unordered_map<string, vector<Session>>& sessions,
                    vector<string>& rooms) {
    int choice;
    while (true) {
        cout << "\n=== Manage Sessions ===\n"
             << "1. Create Session\n"
             << "2. Delete Session\n"
             << "3. Back to Admin Menu\n"
             << "Choose an option: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (choice == 1) {
            // Use existing createSession function
            createSession(modules, sessions, rooms);
        } else if (choice == 2) {
            // List all sessions with an index for deletion
            vector<pair<string, int>> sessionIndex; // pair: <moduleName, session vector index>
            int listIndex = 1;
            for (auto& kv : sessions) {
                for (size_t j = 0; j < kv.second.size(); ++j) {
                    cout << listIndex << ". Module " << kv.first << " - "
                         << kv.second[j].getSessionType() << " on "
                         << kv.second[j].getDay() << " at " << kv.second[j].getTime()
                         << " in room " << kv.second[j].getRoom() << "\n";
                    sessionIndex.push_back({kv.first, j});
                    listIndex++;
                }
            }
            if (sessionIndex.empty()) {
                cout << "No sessions available to delete.\n";
                continue;
            }
            cout << "Select a session to delete by number: ";
            int delChoice;
            cin >> delChoice;
            if (delChoice < 1 || delChoice > (int)sessionIndex.size()) {
                cout << "Invalid selection.\n";
                continue;
            }
            string modName = sessionIndex[delChoice - 1].first;
            int sessIdx = sessionIndex[delChoice - 1].second;
            // Prepare a confirmation message
            string sessionInfo = sessions[modName][sessIdx].getSessionType() + " session for module " +
                                 modName;
            sessions[modName].erase(sessions[modName].begin() + sessIdx);
            if (sessions[modName].empty()) {
                sessions.erase(modName);
            }
            updateSessionsFile(sessionFile, sessions);
            cout << "Deleted " << sessionInfo << ".\n";
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid option.\n";
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

// New function to view the student's timetable by week number.
// Only sessions whose startingWeek <= selected week <= endingWeek and matching student's group are displayed.
void viewStudentTimetable(const unordered_map<string, vector<Session>>& sessions, const string& studentGroup) {
    cout << "Enter week number to view timetable: ";
    int week;
    cin >> week;
    if(cin.fail() || week < 1) {
        cout << "Invalid week number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    bool found = false;
    for (const auto& kv : sessions) {
        for (const auto& s : kv.second) {
            if (s.getStartingWeek() <= week && s.getEndingWeek() >= week && s.getStudentGroup() == studentGroup) {
                s.printSessionDetails();
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
