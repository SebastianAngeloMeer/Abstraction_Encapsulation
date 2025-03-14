#include <iostream>
#include <string>

using namespace std;

// Abstract class (provides abstraction)
class Employee {
private:
    string name;       // Encapsulated data
    int age;          // Encapsulated data
    double salary;     // Encapsulated data

protected:
    Employee(string empName, int empAge, double empSalary) : name(empName), age(empAge), salary(empSalary) {}

public:
    // Pure virtual function (abstraction)
    virtual void calculateBonus() = 0;

    // Getters (encapsulation: controlled access)
    string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    double getSalary() const {
        return salary;
    }

    // Setter for salary (encapsulation)
    void setSalary(double empSalary) {
        if (empSalary > 0) {
            salary = empSalary;
        }
    }
};

// Derived class for Permanent Employee
class PermanentEmployee : public Employee {
public:
    PermanentEmployee(string name, int age, double salary) : Employee(name, age, salary) {}

    void calculateBonus() override {
        double bonus = getSalary() * 0.1; // 10% of salary as bonus
        cout << "Permanent Employee " << getName() << " gets a bonus of: $" << bonus << endl;
    }
};

// Derived class for Contract Employee
class ContractEmployee : public Employee {
public:
    ContractEmployee(string name, int age, double salary) : Employee(name, age, salary) {}

    void calculateBonus() override {
        double bonus = getSalary() * 0.05; // 5% of salary as bonus
        cout << "Contract Employee " << getName() << " gets a bonus of: $" << bonus << endl;
    }
};

class InternEmployee : public Employee {
    
}

int main() {
    PermanentEmployee emp1("John Doe", 30, 50000);
    ContractEmployee emp2("Jane Smith", 25, 30000);

    cout << emp1.getName();

    emp1.calculateBonus();
    emp2.calculateBonus();

    return 0;
}