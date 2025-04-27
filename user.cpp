#include "User.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

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
