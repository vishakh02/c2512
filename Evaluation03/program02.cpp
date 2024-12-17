#include <iostream>
#include <string>
using namespace std;

class Employee {
private:
    int* id;        
    int* age;       
    string* name;   

public:
    
    Employee(int empId, int empAge, const string& empName) {
         cout << "Argument Constructor"<<endl;
        id = new int(empId);             
        age = new int(empAge);           
        name = new string(empName);     
    }

    ~Employee() {
        delete id;      
        delete age;     
        delete name;   
    }

    // Move Constructor
    Employee(Employee&& other) noexcept
        : id(other.id), age(other.age), name(other.name) {
        cout << "\n"; cout << "Destructor called"<<endl;
        other.id = nullptr;
        other.age = nullptr;
        other.name = nullptr;
    }

    Employee& operator=(Employee&& other) noexcept {
        if (this != &other) {
            delete id;     
            delete age;    
            delete name;   

            id = other.id;
            age = other.age;
            name = other.name;

            other.id = nullptr;
            other.age = nullptr;
            other.name = nullptr;
        }
        return *this;
    }

    friend ostream& operator<<(ostream& output, const Employee& emp) {
        output << *(emp.id) << " " << *(emp.age) << " " << *(emp.name);
        return output;
    }

    void swap(Employee& other) {
        Employee temp = std::move(*this);  
        *this = std::move(other);         
        other = std::move(temp);          
    }
};

int main() {

    Employee* e1 = new Employee(101, 22, "Athira");
    Employee* e2 = new Employee(102, 23, "Bhagya");

    cout << "\n" << "Before swapping:" << endl;
    cout << *e1 << endl; // 101 22 Athira
    cout << *e2 << endl; // 102 23 Bhagya

    e1->swap(*e2);

    cout << "\n" << "After swapping:" << endl;
    cout << *e1 << endl; // 102 23 Bhagya
    cout << *e2 << endl; // 101 22 Athira

    delete e1;
    delete e2;

    return 0;
}
