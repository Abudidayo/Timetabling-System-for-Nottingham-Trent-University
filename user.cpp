#include "User.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

// Constructor implementation
User::User(const std::string& username, const std::string& password, const std::string& role, const std::string& group)
    : username(username), password(password), role(role), group(group) {}

// Getter for role
std::string User::getRole() const {
    return role;
}

// Getter for username
std::string User::getUsername() const {
    return username;
}

// Getter for group
std::string User::getGroup() const {
    return group;
}

// Setter for group
void User::setGroup(const std::string& newGroup) {
    group = newGroup;
}

// Load student groups from file
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

// Append a new group if it does not already exist
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

// Delete a group selected by the user
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

// Display available student groups
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

// Manage groups functionality
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
            string newGroup;
            getline(cin, newGroup);
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
            int studentChoice;
            cin >> studentChoice;
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
            int groupChoice;
            cin >> groupChoice;
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
        // if group info doesn't exist, set to empty string
        if (!getline(ss, g, ',')) {
            g = "";
        }
        users[u] = User(u, p, r, g);
    }
    return users;
}

// Append a new user to file
void saveUserToFile(const string& filename, const User& user) {
    ofstream file(filename, ios::app);
    file << user.username << "," << user.password << "," << user.role << "," << user.group << "\n";
}

// Rewrite entire users file with updated group info
void updateAllUsersFile(const string& filename, const unordered_map<string, User>& users) {
    ofstream file(filename);
    for (const auto& kv : users) {
        file << kv.second.username << "," << kv.second.password << ","
             << kv.second.role << "," << kv.second.group << "\n";
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
    User u(username, password, "student", "");
    users[username] = u;
    saveUserToFile(filename, u);
    return true;
}
