#include "Session.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <limits>
#include <string>
using namespace std;

// Default constructor
Session::Session() 
    : sessionType(""), day(""), time(""), room(""), lecturer(""), moduleName(""), studentGroup(""), startingWeek(0), endingWeek(0) {
}

// Constructor with additional student group and week parameters
Session::Session(string sessionType, string day, string time, string room, string lecturer, string moduleName, string studentGroup, int startingWeek, int endingWeek)
    : sessionType(sessionType), day(day), time(time), room(room), lecturer(lecturer), moduleName(moduleName), studentGroup(studentGroup),
      startingWeek(startingWeek), endingWeek(endingWeek) {
}

// Method to print session details
void Session::printSessionDetails() const {
    std::cout << "Session Type: " << sessionType 
              << ", Module: " << moduleName 
              << ", Day: " << day 
              << ", Time: " << time 
              << ", Room: " << room 
              << ", Lecturer: " << lecturer 
              << ", Group: " << studentGroup 
              << ", Weeks: " << startingWeek << " - " << endingWeek 
              << std::endl;
}

// Getter methods
string Session::getSessionType() const { return sessionType; }
string Session::getModuleName() const { return moduleName; }
string Session::getLecturer() const { return lecturer; }
string Session::getRoom() const { return room; }
string Session::getTime() const { return time; }
string Session::getDay() const { return day; }
string Session::getStudentGroup() const { return studentGroup; }
int Session::getStartingWeek() const { return startingWeek; } // new getter
int Session::getEndingWeek() const { return endingWeek; }     // new getter

// Save a session to file with week numbers
void saveSessionToFile(const std::string& filename, const Session& s) {
    std::ofstream file(filename, std::ios::app);
    file << s.getSessionType() << ","
         << s.getDay() << ","
         << s.getTime() << ","
         << s.getRoom() << ","
         << s.getLecturer() << ","
         << s.getModuleName() << ","
         << s.getStudentGroup() << ","
         << s.getStartingWeek() << ","
         << s.getEndingWeek() << "\n";
}

// Load sessions from file reading week numbers
std::unordered_map<std::string, std::vector<Session>> loadSessionsFromFile(const std::string& filename) {
    std::unordered_map<std::string, std::vector<Session>> sessions;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type, day, time, room, lecturer, moduleName, studentGroup, startWeekStr, endWeekStr;
        std::getline(ss, type, ',');
        std::getline(ss, day, ',');
        std::getline(ss, time, ',');
        std::getline(ss, room, ',');
        std::getline(ss, lecturer, ',');
        std::getline(ss, moduleName, ',');
        std::getline(ss, studentGroup, ',');
        std::getline(ss, startWeekStr, ',');
        std::getline(ss, endWeekStr, ',');
        int startWeek = startWeekStr.empty() ? 0 : std::stoi(startWeekStr);
        int endWeek = endWeekStr.empty() ? 0 : std::stoi(endWeekStr);
        sessions[moduleName].emplace_back(type, day, time, room, lecturer, moduleName, studentGroup, startWeek, endWeek);
    }
    return sessions;
}

// Rewrite sessions file based on current sessions map
void updateSessionsFile(const std::string& filename,
                        const std::unordered_map<std::string, std::vector<Session>>& sessions) {
    std::ofstream file(filename);
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
                 << s.getEndingWeek() << "\n";
        }
    }
}

// Create session by picking an existing module
void createSession(const std::unordered_map<std::string, Module>& modules,
                   std::unordered_map<std::string, std::vector<Session>>& sessions,
                   std::vector<std::string>& rooms) {
    if (modules.empty()) {
        std::cout << "No modules available. Add a module first.\n";
        return;
    }

    // Display modules
    std::vector<std::string> names;
    std::cout << "\nAvailable Modules:\n";
    int idx = 1;
    for (const auto& kv : modules) {
        std::cout << "  " << idx << ". " << kv.second.getName() << "\n";
        names.push_back(kv.first);
        ++idx;
    }

    std::cout << "Select a module by number: ";
    int choice; std::cin >> choice;
    if (std::cin.fail() || choice < 1 || choice > static_cast<int>(names.size())) {
        std::cout << "Invalid selection.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::string moduleName = names[choice - 1];

    // Display rooms
    std::cout << "\nAvailable Rooms:\n";
    for (int i = 0; i < static_cast<int>(rooms.size()); ++i) {
        std::cout << "  " << (i + 1) << ". " << rooms[i] << "\n";
    }
    std::cout << "Select a room by number: ";
    int roomChoice; std::cin >> roomChoice;
    if (roomChoice < 1 || roomChoice > static_cast<int>(rooms.size())) {
        std::cout << "Invalid room selection.\n";
        return;
    }
    std::string selectedRoom = rooms[roomChoice - 1];

    // Day-of-week selection
    static const char* days[] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
    std::cout << "\nSelect day:\n";
    for (int i = 0; i < 7; ++i) {
        std::cout << "  " << (i + 1) << ": " << days[i] << "\n";
    }
    std::cout << "Enter choice (1-7): ";
    int dayChoice; std::cin >> dayChoice;
    if (dayChoice < 1 || dayChoice > 7) {
        std::cout << "Invalid day selection.\n";
        return;
    }
    std::string day = days[dayChoice - 1];

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter time of the session: ";
    std::string time; std::getline(std::cin, time);
    std::cout << "Enter lecturer name: ";
    std::string lecturer; std::getline(std::cin, lecturer);

    std::cout << "Enter session type (Lecture/Lab): ";
    std::string type; std::getline(std::cin, type);

    // Load and display student groups from file
    const std::string groupsFile = "student_groups.txt";
    std::vector<std::string> groups;
    {
        std::ifstream file(groupsFile);
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty())
                groups.push_back(line);
        }
    }
    if (groups.empty()) {
        std::cout << "No student groups available. Create a group first.\n";
        return;
    }
    std::cout << "\nAvailable Student Groups:\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << groups[i] << "\n";
    }
    std::cout << "Select a group by number: ";
    int groupChoice; std::cin >> groupChoice;
    if (groupChoice < 1 || groupChoice > static_cast<int>(groups.size())) {
        std::cout << "Invalid group selection.\n";
        return;
    }
    std::string selectedGroup = groups[groupChoice - 1];

    std::cout << "Enter starting week number: ";
    int startingWeek; std::cin >> startingWeek;
    std::cout << "Enter ending week number: ";
    int endingWeek; std::cin >> endingWeek;
    if(std::cin.fail() || startingWeek < 1 || endingWeek < startingWeek) {
        std::cout << "Invalid week numbers provided.\n";
        return;
    }

    Session s(type, day, time, selectedRoom, lecturer, moduleName, selectedGroup, startingWeek, endingWeek);
    sessions[moduleName].push_back(s);
    const std::string sessionFile = "sessions.txt";
    saveSessionToFile(sessionFile, s);

    std::cout << "Session created for module \"" << moduleName
              << "\" on " << day << " at " << time
              << " in room " << selectedRoom
              << ", lecturer " << lecturer
              << ", for group " << selectedGroup
              << ", Weeks " << startingWeek << " to " << endingWeek << ".\n";
}

// View all sessions
void viewAllSessions(const std::unordered_map<std::string, std::vector<Session>>& sessions) {
    if (sessions.empty()) {
        std::cout << "No sessions available.\n";
        return;
    }
    std::cout << "\n=== All Sessions ===\n";
    for (const auto& kv : sessions) {
        for (const auto& sess : kv.second) {
            sess.printSessionDetails();
        }
    }
}

// Manage sessions: create and delete
void manageSessions(const std::unordered_map<std::string, Module>& modules,
                    std::unordered_map<std::string, std::vector<Session>>& sessions,
                    std::vector<std::string>& rooms) {
    int choice;
    const std::string sessionFile = "sessions.txt";
    while (true) {
        std::cout << "\n=== Manage Sessions ===\n"
                  << "1. Create Session\n"
                  << "2. Delete Session\n"
                  << "3. Back to Admin Menu\n"
                  << "Choose an option: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (choice == 1) {
            createSession(modules, sessions, rooms);
        } else if (choice == 2) {
            std::vector<std::pair<std::string, int>> sessionIndex;
            int listIndex = 1;
            for (const auto& kv : sessions) {
                for (size_t j = 0; j < kv.second.size(); ++j) {
                    std::cout << listIndex << ". Module " << kv.first << " - "
                         << kv.second[j].getSessionType() << " on "
                         << kv.second[j].getDay() << " at " << kv.second[j].getTime()
                         << " in room " << kv.second[j].getRoom() << "\n";
                    sessionIndex.push_back({kv.first, static_cast<int>(j)});
                    listIndex++;
                }
            }
            if (sessionIndex.empty()) {
                std::cout << "No sessions available to delete.\n";
                continue;
            }
            std::cout << "Select a session to delete by number: ";
            int delChoice;
            std::cin >> delChoice;
            if (delChoice < 1 || delChoice > static_cast<int>(sessionIndex.size())) {
                std::cout << "Invalid selection.\n";
                continue;
            }
            std::string modName = sessionIndex[delChoice - 1].first;
            int sessIdx = sessionIndex[delChoice - 1].second;
            std::string sessionInfo = sessions[modName][sessIdx].getSessionType() + " session for module " + modName;
            sessions[modName].erase(sessions[modName].begin() + sessIdx);
            if (sessions[modName].empty()) {
                sessions.erase(modName);
            }
            updateSessionsFile(sessionFile, sessions);
            std::cout << "Deleted " << sessionInfo << ".\n";
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Invalid option.\n";
        }
    }
}

// Saves list of rooms to a file.
void saveRoomsToFile(const string& filename, const vector<string>& rooms) {
    ofstream file(filename);
    for (const string& room : rooms) {
        file << room << "\n";
    }
}

// Loads rooms from a file.
vector<string> loadRoomsFromFile(const string& filename) {
    vector<string> rooms;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        rooms.push_back(line);
    }
    return rooms;
}

// Adds a room and saves it.
void addRoom(vector<string>& rooms) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter room name: ";
    string roomName; 
    getline(cin, roomName);
    rooms.push_back(roomName);
    saveRoomsToFile("rooms.txt", rooms);
    cout << "Room \"" << roomName << "\" added.\n";
}

// Manages rooms with options to add, delete, or view them.
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
            case 1: {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter room name: ";
                string roomName; 
                getline(cin, roomName);
                rooms.push_back(roomName);
                saveRoomsToFile("rooms.txt", rooms);
                cout << "Room \"" << roomName << "\" added.\n";
                break;
            }
            case 2: {
                if (rooms.empty()) {
                    cout << "No rooms available to delete.\n";
                    break;
                }
                cout << "\nAvailable Rooms:\n";
                for (size_t i = 0; i < rooms.size(); ++i) {
                    cout << "  " << (i + 1) << ". " << rooms[i] << "\n";
                }
                cout << "Select a room to delete by number: ";
                int roomChoice;
                cin >> roomChoice;
                if (roomChoice < 1 || roomChoice > (int)rooms.size()) {
                    cout << "Invalid selection.\n";
                    break;
                }
                string roomName = rooms[roomChoice - 1];
                rooms.erase(rooms.begin() + (roomChoice - 1));
                saveRoomsToFile("rooms.txt", rooms);
                cout << "Room \"" << roomName << "\" deleted.\n";
                break;
            }
            case 3: {
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
            case 4:
                return;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}
