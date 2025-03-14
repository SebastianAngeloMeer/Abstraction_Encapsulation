#include <iostream>    // For input/output operations
#include <vector>      // For storing employee objects
#include <string>      // For string manipulation
#include <cctype>      // For character type validation functions

using namespace std;

// Abstract base class defining common employee interface
// (Core component for inheritance hierarchy)
class Employee {
protected:
    int id;          // Unique identifier for employee tracking
    string name;     // Employee name storage
    double salary;   // Calculated compensation amount

public:
    // Initializes base employee properties
    Employee(int id, string name, double salary) 
        : id(id), name(name), salary(salary) {}

    // Interface for polymorphic display functionality
    virtual void display() const = 0;

    // Ensures proper cleanup of derived objects
    virtual ~Employee() {}

    // Accessor for ID validation checks
    int getId() const { return id; }
};

// Full-time employee implementation (Fixed salary type)
// (Concrete strategy for salary calculation)
class FullTimeEmployee : public Employee {
public:
    FullTimeEmployee(int id, string name, double salary)
        : Employee(id, name, salary) {}

    // Displays formatted full-time employee data
    void display() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Fixed Monthly Salary: $" << salary << "\n\n";
    }
};

// Part-time employee implementation (Hourly wage type)
// (Concrete strategy for hourly calculation)
class PartTimeEmployee : public Employee {
    double hourlyRate;   // Wage rate storage
    int hoursWorked;     // Time tracking data

public:
    PartTimeEmployee(int id, string name, double hourlyRate, int hoursWorked)
        : Employee(id, name, hourlyRate * hoursWorked), 
          hourlyRate(hourlyRate), hoursWorked(hoursWorked) {}

    // Displays formatted part-time details with calculations
    void display() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Hourly Rate: $" << hourlyRate << "\n";
        cout << "Hours Worked: " << hoursWorked << "\n";
        cout << "Total Salary: $" << salary << "\n\n";
    }
};

// Contractual employee implementation (Project-based payment)
// (Concrete strategy for project-based calculation)
class ContractualEmployee : public Employee {
    double paymentPerProject;  // Project rate storage
    int projectsCompleted;     // Productivity metric

public:
    ContractualEmployee(int id, string name, double paymentPerProject, int projectsCompleted)
        : Employee(id, name, paymentPerProject * projectsCompleted),
          paymentPerProject(paymentPerProject), projectsCompleted(projectsCompleted) {}

    // Displays project-based compensation details
    void display() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Contract Payment Per Project: $" << paymentPerProject << "\n";
        cout << "Projects Completed: " << projectsCompleted << "\n";
        cout << "Total Salary: $" << salary << "\n\n";
    }
};

// Central payroll management system (Facade pattern)
class PayrollSystem {
    vector<Employee*> employees;  // Employee database

    // ID uniqueness validator (Prevents duplicate entries)
    bool isIdUnique(int id) const {
        for (const auto& emp : employees) {
            if (emp->getId() == id) return false;
        }
        return true;
    }

    // Validation subsystem components
    int getValidInt(const string& prompt) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            
            bool valid = !input.empty();
            for (char c : input) {
                if (!isdigit(c)) {  // ctype-based validation
                    valid = false;
                    break;
                }
            }
            
            if (valid) return stoi(input);
            cout << "Invalid input! Please enter whole numbers only.\n";
        }
    }

    double getValidDouble(const string& prompt) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);

            bool valid = !input.empty();
            int decimalPoints = 0;
            for (char c : input) {
                if (c == '.') {
                    if (++decimalPoints > 1) valid = false;
                } else if (!isdigit(c)) {  // ctype check
                    valid = false;
                }
                if (!valid) break;
            }
            
            if (valid) return stod(input);
            cout << "Invalid input! Use numbers with optional single decimal point.\n";
        }
    }

    string getValidName(const string& prompt) {
        string name;
        while (true) {
            cout << prompt;
            getline(cin, name);
            
            bool valid = !name.empty();
            for (char c : name) {
                if (!isalpha(c) && c != ' ') {  // ctype validation
                    valid = false;
                    break;
                }
            }
            
            if (valid) return name;
            cout << "Invalid name! Use letters and spaces only.\n";
        }
    }

public:
    // Employee creation interface (Factory-like method)
    void addEmployee(int type) {
        int id = getValidInt("Enter ID: ");
        while (!isIdUnique(id)) {
            cout << "Duplicate ID! Try again.\n";
            id = getValidInt("Enter ID: ");
        }

        string name = getValidName("Enter Name: ");

        switch (type) {
            case 1: {  // Full-time pathway
                double salary = getValidDouble("Monthly Salary: $");
                employees.push_back(new FullTimeEmployee(id, name, salary));
                break;
            }
            case 2: {  // Part-time pathway
                double rate = getValidDouble("Hourly Rate: $");
                int hours = getValidInt("Hours Worked: ");
                employees.push_back(new PartTimeEmployee(id, name, rate, hours));
                break;
            }
            case 3: {  // Contractual pathway
                double rate = getValidDouble("Payment Per Project: $");
                int projects = getValidInt("Projects Completed: ");
                employees.push_back(new ContractualEmployee(id, name, rate, projects));
                break;
            }
        }
        cout << "Employee added!\n\n";
    }

    // Reporting interface
    void displayPayrollReport() const {
        if (employees.empty()) {
            cout << "No employees in system!\n\n";
            return;
        }
        cout << "\nEmployee Payroll Report ---\n";
        for (const auto& emp : employees) {
            emp->display();  // Polymorphic call
        }
    }

    // Memory management
    ~PayrollSystem() {
        for (auto& emp : employees) {
            delete emp;  // Proper cleanup of polymorphic objects
        }
    }
};

// User interface controller (Main program loop)
int main() {
    PayrollSystem payroll;
    
    while (true) {
        cout << "Payroll System Menu\n";
        cout << "1. Add Full-time Employee\n";
        cout << "2. Add Part-time Employee\n";
        cout << "3. Add Contractual Employee\n";
        cout << "4. Generate Report\n";
        cout << "5. Exit\n";
        
        string choice;
        cout << "Selection: ";
        getline(cin, choice);

        if (choice.length() != 1 || !isdigit(choice[0])) {
            cout << "Invalid menu choice!\n";
            continue;
        }

        switch (choice[0]) {
            case '1': payroll.addEmployee(1); break;
            case '2': payroll.addEmployee(2); break;
            case '3': payroll.addEmployee(3); break;
            case '4': payroll.displayPayrollReport(); break;
            case '5': 
                cout << "Exiting system...\n";
                return 0;
            default:
                cout << "Invalid menu option!\n";
        }
    }
}