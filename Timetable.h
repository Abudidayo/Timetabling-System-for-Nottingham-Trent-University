#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <string>
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

#endif  // TIMETABLE_H