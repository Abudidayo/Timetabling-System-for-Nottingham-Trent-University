#include "Session.h"
#include <iostream>

// Default constructor implementation (initializing with empty/default values)
Session::Session()
    : sessionType(""), day(""), time(""), room(""), lecturer(""), moduleCode(0) {
}

// Constructor with parameters implementation
Session::Session(string sessionType, string day, string time, string room, string lecturer, int moduleCode)
    : sessionType(sessionType), day(day), time(time), room(room), lecturer(lecturer), moduleCode(moduleCode) {
}

// Method to print session details
void Session::printSessionDetails() const {
    cout << "Session Type: " << sessionType << ", Day: " << day << ", Time: " << time
        << ", Room: " << room << ", Lecturer: " << lecturer << ", Module Code: " << moduleCode << endl;
}

// Getter methods
string Session::getSessionType() const { return sessionType; }
int Session::getModuleCode() const { return moduleCode; }
string Session::getLecturer() const { return lecturer; }
string Session::getRoom() const { return room; }
string Session::getTime() const { return time; }
string Session::getDay() const { return day; }
