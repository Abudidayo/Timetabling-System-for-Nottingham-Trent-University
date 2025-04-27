#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Session.h"
#include "Module.h"

using namespace std;

class Session {
private:
    string sessionType;  // Lecture, Lab, etc.
    string day;
    string time;
    string room;
    string lecturer;
    string moduleName; // Associated module
    string studentGroup; // New field for student group
    int startingWeek; // New field for starting week
    int endingWeek;   // New field for ending week

public:
    // Default constructor
    Session();

    // Constructor with parameters
    Session(string sessionType, string day, string time, string room, string lecturer, string moduleName, string studentGroup, int startingWeek, int endingWeek);

    // Method to print session details
    void printSessionDetails() const;

    // Getter methods
    string getSessionType() const;
    string getModuleName() const;
    string getLecturer() const;
    string getRoom() const;
    string getTime() const;
    string getDay() const;
    string getStudentGroup() const;
    int getStartingWeek() const;
    int getEndingWeek() const;
};

void saveSessionToFile(const std::string& filename, const Session& s);
std::unordered_map<std::string, std::vector<Session>> loadSessionsFromFile(const std::string& filename);
void updateSessionsFile(const std::string& filename, const std::unordered_map<std::string, std::vector<Session>>& sessions);
void createSession(const std::unordered_map<std::string, Module>& modules,
                   std::unordered_map<std::string, std::vector<Session>>& sessions,
                   std::vector<std::string>& rooms);
void viewAllSessions(const std::unordered_map<std::string, std::vector<Session>>& sessions);
void manageSessions(const std::unordered_map<std::string, Module>& modules,
                    std::unordered_map<std::string, std::vector<Session>>& sessions,
                    std::vector<std::string>& rooms);

#endif  // SESSION_H
