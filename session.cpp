#include "Session.h"
#include <iostream>


// Default constructor
Session::Session() 
    : sessionType(""), day(""), time(""), room(""), lecturer(""), moduleName(""), studentGroup(""), startingWeek(0), endingWeek(0) {
    // default constructor
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
