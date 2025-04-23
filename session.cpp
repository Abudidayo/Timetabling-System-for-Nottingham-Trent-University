#include "Session.h"
#include <iostream>

// Constructor implementation
Session::Session(string sessionType, string day, string time, string roomNumber, string lecturer)
    : sessionType(sessionType), day(day), time(time), roomNumber(roomNumber), lecturer(lecturer) {
}

// Method to print session details
void Session::printSessionDetails() const {
    cout << "Session Type: " << sessionType << ", Day: " << day << ", Time: " << time
        << ", Room: " << roomNumber << ", Lecturer: " << lecturer << endl;
}

// Getter for room number
string Session::getRoomNumber() const {
    return roomNumber;
}

// Getter for lecturer
string Session::getLecturer() const {
    return lecturer;
}
