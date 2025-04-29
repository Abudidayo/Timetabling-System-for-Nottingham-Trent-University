#ifndef USER_H
#define USER_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class User {
public:
    string username;
    string password;
    string role;   // "admin" or "student"
    string group;  // student group

    User(const string& username = "", 
         const string& password = "", 
         const string& role = "", 
         const string& group = "");

    string getRole() const;
    string getUsername() const;
    string getGroup() const;
    void setGroup(const string& newGroup);
};

// Student group related function declarations (moved from main.cpp)
vector<string> loadStudentGroupsFromFile(const string& filename);
void appendStudentGroup(const string& filename, const string& groupName);
void deleteStudentGroup(const string& filename);
void viewStudentGroups(const string& filename);

// New: manageGroups function declaration moved from main.cpp
void manageGroups(unordered_map<string, User>& users);

// Existing user function declarations
unordered_map<string, User> loadUsersFromFile(const string& filename);
void saveUserToFile(const string& filename, const User& user);
void updateAllUsersFile(const string& filename, const unordered_map<string, User>& users);
bool authenticateUser(const unordered_map<string, User>& users,
                      const string& username,
                      const string& password,
                      string& outRole);
bool registerUser(unordered_map<string, User>& users,
                  const string& filename,
                  const string& username,
                  const string& password);

#endif // USER_H
