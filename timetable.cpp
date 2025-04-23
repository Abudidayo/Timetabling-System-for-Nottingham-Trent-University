#include "Timetable.h"
#include <iostream>

// Add a module to the timetable
void Timetable::addModule(const Module& module) {
    modules.push_back(module);
}

// Add a session to the timetable
void Timetable::addSession(const Session& session) {
    sessions.push_back(session);
}

// Display the timetable with all modules and sessions
void Timetable::displayTimetable() const {
    cout << "Timetable:\n";
    for (const auto& module : modules) {
        module.printDetails();
    }
    for (const auto& session : sessions) {
        session.printSessionDetails();
    }
}
