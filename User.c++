#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

// User roles
enum class UserRole { Admin, Student };

// User data structure
struct User {
    std::string username;
    std::string password;
    UserRole role;
};

// In-memory user database
std::map<std::string, User> userDatabase;

// Function to convert UserRole to string
std::string roleToString(UserRole role) {
    return role == UserRole::Admin ? "Admin" : "Student";
}

// Function to convert string to UserRole
UserRole stringToRole(const std::string& roleStr) {
    return roleStr == "Admin" ? UserRole::Admin : UserRole::Student;
}

// Function to load users from a file
void loadUsersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username, password, roleStr;
        if (iss >> username >> password >> roleStr) {
            userDatabase[username] = { username, password, stringToRole(roleStr) };
        }
    }
    file.close();
}

// Function to save users to a file
void saveUsersToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        return;
    }

    for (const auto& [username, user] : userDatabase) {
        file << user.username << " " << user.password << " " << roleToString(user.role) << "\n";
    }
    file.close();
}

// Function to authenticate user
User* login(const std::string& username, const std::string& password) {
    auto it = userDatabase.find(username);
    if (it != userDatabase.end() && it->second.password == password) {
        return &it->second;
    }
    return nullptr;
}

int main() {
    const std::string filename = "users.txt";

    // Load users from file
    loadUsersFromFile(filename);

    std::string username, password;

    std::cout << "Welcome to the Timetabling System\n";
    std::cout << "Please log in.\n";

    // Input username and password
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    // Authenticate user
    User* user = login(username, password);
    if (user) {
        std::cout << "Login successful! Welcome, " << user->username << ".\n";

        // Redirect based on role
        if (user->role == UserRole::Admin) {
            std::cout << "You are logged in as Admin.\n";
            // Admin functionalities go here
        }
        else if (user->role == UserRole::Student) {
            std::cout << "You are logged in as Student.\n";
            // Student functionalities go here
        }
    }
    else {
        std::cout << "Invalid username or password. Please try again.\n";
    }

    // Save users to file before exiting
    saveUsersToFile(filename);

    return 0;
}
