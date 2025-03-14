#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <limits> // Required for numeric_limits

using namespace std;

class Employee {
protected:
    string id;
    string name;
    double salary;

public:
    Employee(string id, string name, double salary)
        : id(id), name(name), salary(salary) {}

    virtual void display() const = 0;
    virtual ~Employee() {}
    string getId() const { return id; }
};

class FullTimeEmployee : public Employee {
public:
    FullTimeEmployee(string id, string name, double salary)
        : Employee(id, name, salary) {}

    void display() const {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Fixed Monthly Salary: $" << salary << "\n\n";
    }
};

class PartTimeEmployee : public Employee {
    double hourlyRate;
    int hoursWorked;

public:
    PartTimeEmployee(string id, string name, double hourlyRate, int hoursWorked)
        : Employee(id, name, hourlyRate * hoursWorked),
          hourlyRate(hourlyRate), hoursWorked(hoursWorked) {}

    void display() const {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Hourly Rate: $" << hourlyRate << "\n";
        cout << "Hours Worked: " << hoursWorked << "\n";
        cout << "Total Salary: $" << salary << "\n\n";
    }
};

class ContractualEmployee : public Employee {
    double paymentPerProject;
    int projectsCompleted;

public:
    ContractualEmployee(string id, string name, double paymentPerProject, int projectsCompleted)
        : Employee(id, name, paymentPerProject * projectsCompleted),
          paymentPerProject(paymentPerProject), projectsCompleted(projectsCompleted) {}

    void display() const {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Contract Payment Per Project: $" << paymentPerProject << "\n";
        cout << "Projects Completed: " << projectsCompleted << "\n";
        cout << "Total Salary: $" << salary << "\n\n";
    }
};

class PayrollSystem {
    vector<Employee*> employees;

    bool isIdUnique(const string& id) const {
        for(const auto& emp : employees) {
            if(emp->getId() == id) return false;
        }
        return true;
    }

    string getValidID() {
        string input;
        bool isValidInput = false;
        while (!isValidInput) {
            cout << "Enter ID: ";
            getline(cin, input);
            input = trim(input);

            bool valid = !input.empty();
            for(char c : input) {
                if(!isalnum(c)) {
                    valid = false;
                    break;
                }
            }

            if(valid) {
                if(isIdUnique(input)) {
                    isValidInput = true;
                } else {
                    cout << "Duplicate ID! Try again.\n";
                }
            } else {
                cout << "Invalid ID! Use only letters and numbers.\n";
            }
        }
        return input;
    }

    double getValidDouble(const string& prompt) {
        string input;
        double value = 0.0;
        bool isValidInput = false;
        while (!isValidInput) {
            cout << prompt;
            getline(cin, input);
            input = trim(input);

            bool valid = !input.empty();
            int decimalPoints = 0;
            bool hasDigit = false;

            for(char c : input) {
                if(c == '.') {
                    if(++decimalPoints > 1) valid = false;
                } else if(isdigit(c)) {
                    hasDigit = true;
                } else {
                    valid = false;
                }
                if(!valid) break;
            }

            if(valid && hasDigit) {
                try {
                    value = stod(input);
                    isValidInput = true;
                } catch (const std::invalid_argument& e) {
                    cout << "Invalid input! Please enter a valid number.\n";
                } catch (const std::out_of_range& e) {
                    cout << "Input out of range for a double.\n";
                }
            } else {
                cout << "Invalid input! Use numbers with optional single decimal point.\n";
            }
        }
        return value;
    }

    int getValidInt(const string& prompt) {
        string input;
        int value = 0;
        bool isValidInput = false;
        while (!isValidInput) {
            cout << prompt;
            getline(cin, input);
            input = trim(input);

            bool valid = !input.empty();
            for(char c : input) {
                if(!isdigit(c)) {
                    valid = false;
                    break;
                }
            }

            if(valid) {
                try {
                    value = stoi(input);
                    isValidInput = true;
                } catch (const std::invalid_argument& e) {
                    cout << "Invalid input! Please enter a valid integer.\n";
                } catch (const std::out_of_range& e) {
                    cout << "Input out of range for an integer.\n";
                }
            } else {
                cout << "Invalid input! Please enter whole numbers only.\n";
            }
        }
        return value;
    }

    string getValidName() {
        string input;
        bool isValidInput = false;
        while (!isValidInput) {
            cout << "Enter Name: ";
            getline(cin, input);
            input = trim(input);

            bool valid = !input.empty();
            bool prevSpace = false;

            for(char c : input) {
                if(isspace(c)) {
                    if(prevSpace) valid = false;
                    prevSpace = true;
                } else {
                    if(!isalpha(c)) valid = false;
                    prevSpace = false;
                }
                if(!valid) break;
            }

            if(valid) {
                isValidInput = true;
            } else {
                cout << "Invalid name! Use letters and single spaces between names.\n";
            }
        }
        return input;
    }

public:
    string trim(const string& str) {
        if(str.empty()) return "";

        size_t start = 0;
        size_t end = str.length() - 1;

        while(start <= end && isspace(str[start])) start++;
        while(end >= start && isspace(str[end])) end--;

        return str.substr(start, end - start + 1);
    }

    void addEmployee(int type) {
        string id = getValidID();
        string name = getValidName();

        switch(type) {
            case 1: {
                double salary = getValidDouble("Monthly Salary: $");
                employees.push_back(new FullTimeEmployee(id, name, salary));
                break;
            }
            case 2: {
                double rate = getValidDouble("Hourly Rate: $");
                int hours = getValidInt("Hours Worked: ");
                employees.push_back(new PartTimeEmployee(id, name, rate, hours));
                break;
            }
            case 3: {
                double rate = getValidDouble("Payment Per Project: $");
                int projects = getValidInt("Projects Completed: ");
                employees.push_back(new ContractualEmployee(id, name, rate, projects));
                break;
            }
        }
        cout << "Employee added!\n\n";
    }

    void displayPayrollReport() const {
        if(employees.empty()) {
            cout << "No employees in system!\n\n";
            return;
        }
        cout << "\nEmployee Payroll Report ---\n";
        for(const auto& emp : employees) {
            emp->display();
        }
    }

    ~PayrollSystem() {
        for(auto& emp : employees) {
            delete emp;
        }
    }
};

int main() {
    PayrollSystem payroll;
    bool running = true;

    while(running) {
        cout << "Payroll System Menu\n";
        cout << "1. Add Full-time Employee\n";
        cout << "2. Add Part-time Employee\n";
        cout << "3. Add Contractual Employee\n";
        cout << "4. Generate Report\n";
        cout << "5. Exit\n";

        string choice;
        cout << "Selection: ";
        getline(cin, choice);
        choice = payroll.trim(choice);

        if(choice.length() != 1 || !isdigit(choice[0])) {
            cout << "Invalid menu choice!\n";
            continue;
        }

        switch(choice[0]) {
            case '1': payroll.addEmployee(1); break;
            case '2': payroll.addEmployee(2); break;
            case '3': payroll.addEmployee(3); break;
            case '4': payroll.displayPayrollReport(); break;
            case '5':
                cout << "Exiting system...\n";
                running = false;
                break;
            default:
                cout << "Invalid menu option!\n";
        }
    }
    return 0;
}