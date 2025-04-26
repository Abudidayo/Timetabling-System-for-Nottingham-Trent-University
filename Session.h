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

public:
    // Default constructor
    Session();

    // Constructor with parameters
    Session(string sessionType, string day, string time, string room, string lecturer, string moduleName);

    // Method to print session details
    void printSessionDetails() const;

    // Getter methods
    string getSessionType() const;
    string getModuleName() const;
    string getLecturer() const;
    string getRoom() const;
    string getTime() const;
    string getDay() const;
};

#endif  // SESSION_H
