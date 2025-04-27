#ifndef USER_H
#define USER_H

#include <string>
#include <unordered_map>

class User {
public:
    std::string username;
    std::string password;
    std::string role;   // "admin" or "student"
    std::string group;  // student group

    // Constructor with default parameter values
    User(const std::string& username = "", 
         const std::string& password = "", 
         const std::string& role = "", 
         const std::string& group = "");

    // Member functions
    std::string getRole() const;
    std::string getUsername() const;
    std::string getGroup() const;
    void setGroup(const std::string& newGroup);
};

// Free functions declarations
std::unordered_map<std::string, User> loadUsersFromFile(const std::string& filename);
void saveUserToFile(const std::string& filename, const User& user);
void updateAllUsersFile(const std::string& filename, const std::unordered_map<std::string, User>& users);
bool authenticateUser(const std::unordered_map<std::string, User>& users,
                      const std::string& username,
                      const std::string& password,
                      std::string& outRole);
bool registerUser(std::unordered_map<std::string, User>& users,
                  const std::string& filename,
                  const std::string& username,
                  const std::string& password);

#endif // USER_H
