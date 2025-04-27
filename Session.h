#ifndef SESSION_H
#define SESSION_H

#include <string>
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

#endif  // SESSION_H
