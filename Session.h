#ifndef SESSION_H
#define SESSION_H

#include <string>
using namespace std;

class Session {
private:
    string sessionType;  // Lecture, Lab, etc.
    string day;
    string time;
    string roomNumber;   // New variable for the room number
    string lecturer;     // New variable for the lecturer's name

public:
    // Constructor with added roomNumber and lecturer
    Session(string sessionType, string day, string time, string roomNumber, string lecturer);

    // Method to print session details
    void printSessionDetails() const;

    // Getter methods for room number and lecturer
    string getRoomNumber() const;
    string getLecturer() const;
};

#endif  // SESSION_H
