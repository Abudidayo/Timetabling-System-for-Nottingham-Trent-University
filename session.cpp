#include "Session.h"
#include <iostream>


// Constructor with additional student group parameter
Session::Session(string sessionType, string day, string time, string room, string lecturer, string moduleName, string studentGroup)
    : sessionType(sessionType), day(day), time(time), room(room), lecturer(lecturer), moduleName(moduleName), studentGroup(studentGroup) {
}

// Method to print session details
void Session::printSessionDetails() const {
    cout << "Session Type: " << sessionType << ", Day: " << day << ", Time: " << time
         << ", Room: " << room << ", Lecturer: " << lecturer << ", Module Name: " << moduleName
         << ", Student Group: " << studentGroup << endl; // Include student group
}

// Getter methods
string Session::getSessionType() const { return sessionType; }
string Session::getModuleName() const { return moduleName; }
string Session::getLecturer() const { return lecturer; }
string Session::getRoom() const { return room; }
string Session::getTime() const { return time; }
string Session::getDay() const { return day; }
string Session::getStudentGroup() const { return studentGroup; }
