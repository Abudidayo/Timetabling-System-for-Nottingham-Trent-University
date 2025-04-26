#include "Session.h"
#include <iostream>

// Default constructor implementation (initializing with empty/default values)
Session::Session()
    : sessionType(""), day(""), time(""), room(""), lecturer(""), moduleName("") {
}

// Constructor with parameters implementation
Session::Session(string sessionType, string day, string time, string room, string lecturer, string moduleName)
    : sessionType(sessionType), day(day), time(time), room(room), lecturer(lecturer), moduleName(moduleName) {
}

// Method to print session details
void Session::printSessionDetails() const {
    cout << "Session Type: " << sessionType << ", Day: " << day << ", Time: " << time
        << ", Room: " << room << ", Lecturer: " << lecturer << ", Module Name: " << moduleName << endl;
}

// Getter methods
string Session::getSessionType() const { return sessionType; }
string Session::getModuleName() const { return moduleName; }
string Session::getLecturer() const { return lecturer; }
string Session::getRoom() const { return room; }
string Session::getTime() const { return time; }
string Session::getDay() const { return day; }
