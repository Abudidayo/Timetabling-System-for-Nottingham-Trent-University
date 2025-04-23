#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
private:
    string username;
    string password;
    string role;  // "admin" or "student"
    string group; // Group name assigned to the student (CS1, CS2, etc.)

public:
    // Constructor
    User(string username, string password, string role, string group = "");

    // Authentication method
    bool authenticate(string enteredPassword);

    // Getter methods
    string getRole() const;
    string getUsername() const;
    string getGroup() const;  // Getter for group

    // Setter method to assign a group
    void setGroup(const string& newGroup);
};

#endif  // USER_H
