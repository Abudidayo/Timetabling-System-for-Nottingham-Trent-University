#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>  // For better formatting of the menu

using namespace std;

// Struct to represent a user
struct User {
    string username;
    string password;
    string role; // "admin" or "student"
};

// Function to load users from a file
unordered_map<string, User> loadUsersFromFile(const string& filename) {
    unordered_map<string, User> users;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string username, password, role;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, role);

        User user = { username, password, role };
        users[username] = user;
    }
    return users;
}

// Function to save a new user to the file
void saveUserToFile(const string& filename, const User& user) {
    ofstream file(filename, ios::app);
    file << user.username << "," << user.password << "," << user.role << endl;
}

// Function to display the TUI menu for the admin role
void showAdminMenu() {
    cout << "\n=== Admin Menu ===\n";
    cout << "1. Add Module\n";
    cout << "2. Manage Groups\n";
    cout << "3. View Timetable\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

// Function to display the TUI menu for the student role
void showStudentMenu() {
    cout << "\n=== Student Menu ===\n";
    cout << "1. View Timetable\n";
    cout << "2. Search Timetable\n";
    cout << "3. Export Timetable\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

// Function to authenticate the user
bool authenticateUser(const unordered_map<string, User>& users, const string& username, const string& password, string& role) {
    auto it = users.find(username);
    if (it != users.end() && it->second.password == password) {
        role = it->second.role;
        return true;
    }
    return false;
}

// Function to register a new user (as student)
bool registerUser(unordered_map<string, User>& users, const string& username, const string& password) {
    if (users.find(username) != users.end()) {
        cout << "Username already exists. Please choose a different username.\n";
        return false;
    }

    // Add new user to the map and file
    User newUser = { username, password, "student" };
    users[username] = newUser;
    saveUserToFile("users.txt", newUser);
    return true;
}

int main() {
    string filename = "users.txt";
    unordered_map<string, User> users = loadUsersFromFile(filename);

    string username, password, role;
    int choice;

    cout << "Welcome to the Timetabling System\n";
    cout << "1. Login\n";
    cout << "2. Register as Student\n";
    cout << "Choose an option: ";
    cin >> choice;

    if (choice == 2) {
        cout << "Enter a username: ";
        cin >> username;
        cout << "Enter a password: ";
        cin >> password;

        // Register the user
        if (registerUser(users, username, password)) {
            cout << "Registration successful! Now you can log in.\n";
        }
        else {
            return 0; // Exit if registration fails (e.g., username already exists)
        }
    }

    // Loop for login attempts
    bool loginSuccess = false;
    int attemptCount = 0;
    const int maxAttempts = 3; // Maximum attempts before exiting

    while (!loginSuccess && attemptCount < maxAttempts) {
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        if (authenticateUser(users, username, password, role)) {
            cout << "Login successful!\n";
            loginSuccess = true;
        } else {
            attemptCount++;
            cout << "Invalid username or password. You have " << (maxAttempts - attemptCount) << " attempts left.\n";
            if (attemptCount == maxAttempts) {
                cout << "Maximum attempts reached. Exiting program.\n";
                return 0; // Exit after max failed attempts
            }
        }
    }

    // Role-specific menu
    if (role == "admin") {
        int option;
        while (true) {
            showAdminMenu();
            cin >> option;
            switch (option) {
                case 1: cout << "Adding Module...\n"; break;
                case 2: cout << "Managing Groups...\n"; break;
                case 3: cout << "Viewing Timetable...\n"; break;
                case 4: cout << "Exiting...\n"; return 0;
                default: cout << "Invalid option. Try again.\n"; break;
            }
        }
    }
    else if (role == "student") {
        int option;
        while (true) {
            showStudentMenu();
            cin >> option;
            switch (option) {
                case 1: cout << "Viewing Timetable...\n"; break;
                case 2: cout << "Searching Timetable...\n"; break;
                case 3: cout << "Exporting Timetable...\n"; break;
                case 4: cout << "Exiting...\n"; return 0;
                default: cout << "Invalid option. Try again.\n"; break;
            }
        }
    }

    return 0;
}
