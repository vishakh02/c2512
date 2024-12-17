#include <iostream>
#include <string>
using namespace std;

class Employee {
private:
    int id;        
    int age;       
    string name;   

public:
    // Parameterized Constructor
    Employee(int empId, int empAge, const string& empName) 
        : id(empId), age(empAge), name(empName) {
            
            cout << "Argument Constructor" <<endl;
        }

    // Move Constructor
    Employee(Employee&& other) noexcept
        : id(other.id), age(other.age), name(std::move(other.name)) {
        cout << "\n" << "Move Constructor" << endl;
        other.id = 0;
        other.age = 0;
    }

    // Move Assignment Operator
    Employee& operator=(Employee&& other) noexcept {
        if (this != &other) {
            
            id = other.id;
            age = other.age;
            name = std::move(other.name);
            
            other.id = 0;
            other.age = 0;
        }
        return *this;
    }

    // Overload << operator for displaying Employee details
    friend ostream& operator<<(ostream& output, const Employee& emp) {
        output << emp.id << " " << emp.age << " " << emp.name;
        return output;
    }

    
    void swap(Employee& other) {
        
        Employee temp = std::move(*this);  
        *this = std::move(other);          
         other = std::move(temp);           
    }
};

int main() {
    // Creating Employee objects
    Employee e1(101, 22, "Athira");
    Employee e2(102, 23, "Bhagya");

    cout << "\n"; cout << "Before swapping:" << endl;
    cout << e1 << endl; // 101 22 Athira
    cout << e2 << endl; // 102 23 Bhagya

    e1.swap(e2);
    
     cout << "\n"; cout << "After swapping:" << endl;
    cout << e1 << endl; // 102 23 Bhagya
    cout << e2 << endl; // 101 22 Athira

    return 0;
}
