#include "Timetable.h"
#include "Session.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <limits>
#include <string>
using namespace std;

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

// Views the student's timetable for a specified week.
void viewStudentTimetable(const unordered_map<string, vector<Session>>& sessions, const string& studentGroup) {
    cout << "Enter week number to view timetable: ";
    int week;
    cin >> week;
    if (cin.fail() || week < 1) {
        cout << "Invalid week number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cout << "\n=== Timetable for Week " << week << " ===\n";
    bool found = false;
    for (const auto& kv : sessions) {
        for (const auto& s : kv.second) {
            if (s.getStartingWeek() <= week && s.getEndingWeek() >= week && s.getStudentGroup() == studentGroup) {
                cout << s.getDay() << " | " << s.getSessionType() << " from " << s.getTime()
                     << " in " << s.getRoom() << " with " << s.getLecturer() << "\n";
                found = true;
            }
        }
    }
    if (!found)
        cout << "No sessions available for week " << week << " in your group.\n";
}
