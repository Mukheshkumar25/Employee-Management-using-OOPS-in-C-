#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>

using namespace std;

const string ATTENDANCE_FILE = "attendance.txt";
const string EMPLOYEE_DATA_FILE = "employee_data.txt";

vector<string> employeeIDs;
int totalEmployees = 0;

// Function to load employee data from the file
void loadEmployeeData() {
    ifstream inFile(EMPLOYEE_DATA_FILE);

    if (!inFile) {
        cerr << "Error opening " << EMPLOYEE_DATA_FILE << " for reading." << endl;
        return;
    }

    string employeeID, employeeName;

    while (inFile >> employeeID >> employeeName) {
        employeeIDs.push_back(employeeID);
        totalEmployees++;
    }

    inFile.close();
}

// Function to greet the user
void greet() {
    cout << "Good morning!\n";
}

// Function to mark attendance for an employee
void markAttendance(const string& employeeID) {
    // Check if the employee ID is present in the employee data
    if (find(employeeIDs.begin(), employeeIDs.end(), employeeID) == employeeIDs.end()) {
        cout << "Error: Employee ID not found. Please enter a valid Employee ID.\n";
        return;
    }

    ofstream outFile(ATTENDANCE_FILE, ios::app);

    if (!outFile) {
        cerr << "Error opening " << ATTENDANCE_FILE << " for writing." << endl;
        return;
    }

    auto currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    outFile << employeeID << " " << currentTime << endl;

    cout << "Attendance marked for employee ID: " << employeeID << endl;

    outFile.close();
}

// Function to display attendance for all employees
void displayAttendance() {
    ifstream inFile(ATTENDANCE_FILE);

    if (!inFile) {
        cerr << "Error opening " << ATTENDANCE_FILE << " for reading." << endl;

        // Check if there are no employees added
        if (totalEmployees == 0) {
            cout << "No employees added. Please add employees first." << endl;
        }

        return;
    }

    // Check if the file is empty
    inFile.seekg(0, ios::end);
    if (inFile.tellg() == 0) {
        cout << "No attendance records found." << endl;
        inFile.close();
        return;
    }

    // Reset the file pointer to the beginning
    inFile.seekg(0, ios::beg);

    string employeeID;
    long long timestamp;

    cout << "Employee ID\tAttendance Time" << endl;
    cout << "---------------------------------" << endl;

    while (inFile >> employeeID >> timestamp) {
        // Convert the timestamp to a struct tm for correct formatting
        struct tm *timeInfo = localtime(reinterpret_cast<time_t*>(&timestamp));

        // Output without the newline character from asctime
        cout << setw(12) << employeeID << "\t" << asctime(timeInfo) << flush;
    }

    inFile.close();
}

// Function to clear all attendance records
void clearAttendance() {
    ofstream outFile(ATTENDANCE_FILE, ios::trunc);

    if (!outFile) {
        cerr << "Error opening " << ATTENDANCE_FILE << " for clearing attendance." << endl;
        return;
    }

    cout << "Attendance records cleared." << endl;

    outFile.close();
}

// Function to clear attendance for a specific employee
void clearEmployeeAttendance(const string& employeeID) {
    ifstream inFile(ATTENDANCE_FILE);
    ofstream outFile("temp.txt", ios::out);

    if (!inFile || !outFile) {
        cerr << "Error opening files for clearing employee attendance." << endl;
        return;
    }

    string currentID;
    long long timestamp;

    while (inFile >> currentID >> timestamp) {
        if (currentID != employeeID) {
            outFile << currentID << " " << timestamp << endl;
        }
    }

    inFile.close();
    outFile.close();

    remove(ATTENDANCE_FILE.c_str());
    rename("temp.txt", ATTENDANCE_FILE.c_str());

    cout << "Attendance records cleared for employee ID: " << employeeID << endl;
}

// Function to add employee details (ID and name) to a file
void addEmployeeData(const string& employeeID, const string& employeeName) {
    ofstream outFile(EMPLOYEE_DATA_FILE, ios::app);

    if (!outFile) {
        cerr << "Error opening " << EMPLOYEE_DATA_FILE << " for adding employee data." << endl;
        return;
    }

    outFile << employeeID << " " << employeeName << endl;

    cout << "Employee data added for ID: " << employeeID << endl;

    // Keep track of added employee IDs
    employeeIDs.push_back(employeeID);

    totalEmployees++; // Increment totalEmployees

    outFile.close();
}

// Function to display employee data
void displayEmployeeData() {
    ifstream inFile(EMPLOYEE_DATA_FILE);

    if (!inFile) {
        cerr << "Error opening " << EMPLOYEE_DATA_FILE << " for reading." << endl;
        return;
    }

    string employeeID, employeeName;

    cout << "Employee ID\tEmployee Name" << endl;
    cout << "---------------------------------" << endl;

    while (inFile >> employeeID >> employeeName) {
        cout << setw(12) << employeeID << "\t" << employeeName << endl;
    }

    inFile.close();
}

// Function to remove an employee from the system
void removeEmployee(const string& employeeID) {
    ifstream inFile(EMPLOYEE_DATA_FILE);
    ofstream outFile("temp.txt", ios::out);

    if (!inFile || !outFile) {
        cerr << "Error opening files for removing employee." << endl;
        return;
    }

    string currentID, employeeName;
    bool found = false;

    while (inFile >> currentID >> employeeName) {
        if (currentID != employeeID) {
            outFile << currentID << " " << employeeName << endl;
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();

    remove(EMPLOYEE_DATA_FILE.c_str());
    rename("temp.txt", EMPLOYEE_DATA_FILE.c_str());

    if (found) {
        cout << "Employee ID " << employeeID << " removed from the system." << endl;
        employeeIDs.erase(remove(employeeIDs.begin(), employeeIDs.end(), employeeID), employeeIDs.end());
        totalEmployees--; // Decrement totalEmployees
    } else {
        cout << "Employee ID " << employeeID << " not found." << endl;
    }
}

int main() {
    loadEmployeeData(); // Load existing employee data at the start

    greet();

    int choice;
    string employeeID, employeeName;

    do {
        cout << "\n1. Add Employee\n2. Mark Attendance\n3. Display Attendance\n4. Clear Attendance\n5. Clear Specific Employee Attendance\n";
        cout << "6. Display Employee Data\n7. Remove Employee\n8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Employee ID: ";
                cin >> employeeID;
                cout << "Enter Employee Name: ";
                cin >> employeeName;
                addEmployeeData(employeeID, employeeName);
                break;
            case 2:
                cout << "Enter Employee ID: ";
                cin >> employeeID;
                markAttendance(employeeID);
                break;
            case 3:
                displayAttendance();
                break;
            case 4:
                clearAttendance();
                break;
            case 5:
                cout << "Enter Employee ID: ";
                cin >> employeeID;
                clearEmployeeAttendance(employeeID);
                break;
            case 6:
                displayEmployeeData();
                break;
            case 7:
                cout << "Enter Employee ID: ";
                cin >> employeeID;
                removeEmployee(employeeID);
                break;
            case 8:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
