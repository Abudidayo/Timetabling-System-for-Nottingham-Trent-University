#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Module.h"
#include "Session.h"

using namespace std;

class Timetable {
private:
    vector<Module> modules;
    vector<Session> sessions;

public:
    // Methods to manage the timetable
    void addModule(const Module& module);
    void addSession(const Session& session);
    void displayTimetable() const;
};

class Session; // forward declaration if not included

// Declaration for viewing the student's timetable.
void viewStudentTimetable(const unordered_map<string, vector<Session>>& sessions, const string& studentGroup);

#endif  // TIMETABLE_H