#include <iostream>    // For input/output operations
#include <vector>      // For dynamic array storage of Employee pointers
#include <string>      // For string handling
#include <cctype>      // For character validation functions like isdigit() and isalpha()

using namespace std;

// ------------------------------ ABSTRACTION -------------------------------------
// The Employee class is an Abstract class — it defines a common interface (blueprint)
// for all types of employees, but leaves the implementation of the display() method
// to the derived classes. It hides the internal complexity and only exposes necessary behaviors.
// --------------------------------------------------------------------------------

class Employee {
protected:
    int id;          // Unique identifier for employee tracking
    string name;     // Employee name
    double salary;   // Common salary variable (actual value varies depending on employee type)

public:
    // --------------------- ENCAPSULATION ----------------------------------------
    // Variables id, name, and salary are marked as protected.
    // They are accessible only inside this class and its derived classes,
    // and not accessible directly from the outside (e.g., from main()).
    // ---------------------------------------------------------------------------

    // Constructor to initialize common attributes of any employee
    Employee(int id, string name, double salary) 
        : id(id), name(name), salary(salary) {}

    // Abstract method (pure virtual function) — forces derived classes to implement this
    virtual void display() const = 0;

    // Virtual destructor ensures proper cleanup of derived class objects during deletion
    virtual ~Employee() {}

    // Accessor method for employee ID (part of encapsulation)
    int getId() const { return id; }
};

// --------------------------- FULL-TIME EMPLOYEE CLASS ----------------------------
class FullTimeEmployee : public Employee {
    // Inherits id, name, and salary from Employee
public:
    // Constructor initializes full-time employee with fixed salary
    FullTimeEmployee(int id, string name, double salary)
        : Employee(id, name, salary) {}

    // Concrete implementation of display() for full-time employees
    void display() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Fixed Monthly Salary: $" << salary << "\n\n";
    }
};

// --------------------------- PART-TIME EMPLOYEE CLASS ----------------------------
class PartTimeEmployee : public Employee {
    double hourlyRate;   // Hourly pay rate
    int hoursWorked;     // Number of hours worked

public:
    // Constructor calculates total salary based on hourlyRate * hoursWorked
    PartTimeEmployee(int id, string name, double hourlyRate, int hoursWorked)
        : Employee(id, name, hourlyRate * hoursWorked), 
          hourlyRate(hourlyRate), hoursWorked(hoursWorked) {}

    // Concrete implementation of display() for part-time employees
    void display() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Hourly Rate: $" << hourlyRate << "\n";
        cout << "Hours Worked: " << hoursWorked << "\n";
        cout << "Total Salary: $" << salary << "\n\n";
    }
};

// ------------------------ CONTRACTUAL EMPLOYEE CLASS ----------------------------
class ContractualEmployee : public Employee {
    double paymentPerProject;  // Pay rate per project
    int projectsCompleted;     // Number of completed projects

public:
    // Constructor calculates total salary based on projects completed
    ContractualEmployee(int id, string name, double paymentPerProject, int projectsCompleted)
        : Employee(id, name, paymentPerProject * projectsCompleted),
          paymentPerProject(paymentPerProject), projectsCompleted(projectsCompleted) {}

    // Concrete implementation of display() for contractual employees
    void display() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Contract Payment Per Project: $" << paymentPerProject << "\n";
        cout << "Projects Completed: " << projectsCompleted << "\n";
        cout << "Total Salary: $" << salary << "\n\n";
    }
};

// ------------------------- PAYROLL SYSTEM (FACADE CLASS) ------------------------
// This class acts as the interface for managing employee records.
// It simplifies complex interactions like creation, validation, and reporting.
// ------------------------------------------------------------------------------
class PayrollSystem {
    vector<Employee*> employees;  // Vector holding pointers to various Employee objects

    // Check for duplicate employee ID
    bool isIdUnique(int id) const {
        for (const auto& emp : employees) {
            if (emp->getId() == id) return false;
        }
        return true;
    }

    // ----------------- VALIDATION METHODS (INPUT HANDLING) ----------------------

    // Get a valid integer input (e.g., ID, hours worked, number of projects)
    int getValidInt(const string& prompt) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);

            bool valid = !input.empty();
            for (char c : input) {
                if (!isdigit(c)) {
                    valid = false;
                    break;
                }
            }

            if (valid) return stoi(input);
            cout << "Invalid input! Please enter whole numbers only.\n";
        }
    }

    // Get a valid double input (e.g., salary, hourly rate)
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
                } else if (!isdigit(c)) {
                    valid = false;
                }
                if (!valid) break;
            }

            if (valid) return stod(input);
            cout << "Invalid input! Use numbers with optional single decimal point.\n";
        }
    }

    // Get a valid name input (letters and spaces only)
    string getValidName(const string& prompt) {
        string name;
        while (true) {
            cout << prompt;
            getline(cin, name);

            bool valid = !name.empty();
            for (char c : name) {
                if (!isalpha(c) && c != ' ') {
                    valid = false;
                    break;
                }
            }

            if (valid) return name;
            cout << "Invalid name! Use letters and spaces only.\n";
        }
    }

public:
    // ----------------- EMPLOYEE CREATION FUNCTION -------------------------------
    // Based on the employee type selected in main(), this method collects necessary
    // information (ID, name, salary/rate/hours) and creates the appropriate object.
    void addEmployee(int type) {
        int id = getValidInt("Enter ID: ");
        while (!isIdUnique(id)) {
            cout << "Duplicate ID! Try again.\n";
            id = getValidInt("Enter ID: ");
        }

        string name = getValidName("Enter Name: ");

        // Depending on type, construct appropriate derived class object
        switch (type) {
            case 1: {  // Full-time employee
                double salary = getValidDouble("Monthly Salary: $");
                employees.push_back(new FullTimeEmployee(id, name, salary));
                break;
            }
            case 2: {  // Part-time employee
                double rate = getValidDouble("Hourly Rate: $");
                int hours = getValidInt("Hours Worked: ");
                employees.push_back(new PartTimeEmployee(id, name, rate, hours));
                break;
            }
            case 3: {  // Contractual employee
                double rate = getValidDouble("Payment Per Project: $");
                int projects = getValidInt("Projects Completed: ");
                employees.push_back(new ContractualEmployee(id, name, rate, projects));
                break;
            }
        }
        cout << "Employee added!\n\n";
    }

    // -------------------- PAYROLL REPORT GENERATION ------------------------------
    // Iterates over the employee list and calls the polymorphic display() method
    void displayPayrollReport() const {
        if (employees.empty()) {
            cout << "No employees in system!\n\n";
            return;
        }

        cout << "\nEmployee Payroll Report ---\n";
        for (const auto& emp : employees) {
            emp->display();  // Polymorphic behavior: appropriate display() is called
        }
    }

    // Destructor: Frees memory by deleting all dynamically created employee objects
    ~PayrollSystem() {
        for (auto& emp : employees) {
            delete emp;
        }
    }
};

// ---------------------- MAIN FUNCTION (USER INTERFACE) ---------------------------
int main() {
    PayrollSystem payroll;  // PayrollSystem object that manages everything

    while (true) {
        // Main menu interface
        cout << "Payroll System Menu\n";
        cout << "1. Add Full-time Employee\n";
        cout << "2. Add Part-time Employee\n";
        cout << "3. Add Contractual Employee\n";
        cout << "4. Generate Report\n";
        cout << "5. Exit\n";

        string choice;
        cout << "Selection: ";
        getline(cin, choice);  // Read user input

        // Input validation for menu choice
        if (choice.length() != 1 || !isdigit(choice[0])) {
            cout << "Invalid menu choice!\n";
            continue;
        }

        // Based on the menu choice, call appropriate functions from PayrollSystem
        switch (choice[0]) {
            case '1': 
                // Calls PayrollSystem::addEmployee with type 1 (Full-time)
                // -> Collects info and stores in FullTimeEmployee object
                payroll.addEmployee(1); 
                break;

            case '2':
                // Calls PayrollSystem::addEmployee with type 2 (Part-time)
                // -> Collects rate and hours, creates PartTimeEmployee object
                payroll.addEmployee(2); 
                break;

            case '3':
                // Calls PayrollSystem::addEmployee with type 3 (Contractual)
                // -> Collects project rate and number of projects
                payroll.addEmployee(3); 
                break;

            case '4':
                // Generates a full payroll report using displayPayrollReport()
                payroll.displayPayrollReport(); 
                break;

            case '5':
                // Exit the program gracefully
                cout << "Exiting system...\n";
                return 0;

            default:
                cout << "Invalid menu option!\n";
        }
    }
}
