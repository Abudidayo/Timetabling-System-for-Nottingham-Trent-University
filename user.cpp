#include "User.h"

// Constructor implementation
User::User(string username, string password, string role, string group)
    : username(username), password(password), role(role), group(group) {}

// Authenticate the user by comparing passwords
bool User::authenticate(string enteredPassword) {
    return enteredPassword == password;
}

// Getter for role
string User::getRole() const {
    return role;
}

// Getter for username
string User::getUsername() const {
    return username;
}

// Getter for group
string User::getGroup() const {
    return group;
}

// Setter for group
void User::setGroup(const string& newGroup) {
    group = newGroup;
}
