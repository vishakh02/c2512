#include <iostream>
#include <cstring>

using namespace std;


class Employee {
protected:
    int* Id;
    double* age;
    char* name;

public:
    virtual void swap(Employee& other);
    Employee(int Id, double age, const char* name);
    virtual ~Employee();
    
    friend ostream& operator<<(ostream& output, const Employee& employee);
};

class Programmer : public Employee {
private:
    string* tasks;
    int taskCount;
    
public:
    void swap(Programmer& other) ;
    Programmer(int Id, double age, const char* name, string* tasks, int taskCount);
    ~Programmer();
    
    friend ostream& operator<<(ostream& output, const Programmer& programmer);
};

int main() {
    
    string tasks1[] = {"Task01", "Task02", "Task03"};
    Employee* p1 = new Programmer(101, 22, "Athira", tasks1, 3);
    
    string tasks2[] = {"Task0A", "Task0B", "Task0C"};
    Employee* p2 = new Programmer(102, 23, "Bhagya", tasks2, 3);

    cout << "\nOriginal values: " << endl;
    cout << *p1 << endl;
    cout << *p2 << endl;

    p1->swap(*p2);

    cout << "\nSwapped values: " << endl;
    cout << *p1 << endl;
    cout << *p2 << endl;
    cout << "\n";

    delete p1;
    delete p2;

    return 0;
}

// Employee class methods
void Employee::swap(Employee& other) {
    int tempId = *this->Id;
    *this->Id = *other.Id;
    *other.Id = tempId;

    double tempAge = *this->age;
    *this->age = *other.age;
    *other.age = tempAge;

    char* tempName = this->name;  
    this->name = other.name;   
    other.name = tempName;       
}

Employee::Employee(int Id, double age, const char* name) {
    cout << "Employee Constructor Invoked" << endl;
    
    this->Id = new int(Id);
    this->age = new double(age);
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

Employee::~Employee() {
    cout << "Employee Destructor Invoked" << endl;
    delete Id;
    delete age;
    delete[] name;
}

ostream& operator<<(ostream& output, const Employee& employee) {
    output << *employee.Id << ", " << *employee.age << ", " << employee.name;
    return output;
}

// Programmer class methods
void Programmer::swap(Programmer& other) {
    Employee::swap(static_cast<Employee&>(other));  // Use base class swap directly
    
    string* tempTasks = this->tasks;
    this->tasks = other.tasks;
    other.tasks = tempTasks;
}

Programmer::Programmer(int Id, double age, const char* name, string* tasks, int taskCount)
    : Employee(Id, age, name), taskCount(taskCount) {
    cout << "Programmer Constructor called" << endl;
    this->tasks = new string[taskCount];
    for (int i = 0; i < taskCount; ++i) {
        this->tasks[i] = tasks[i];
    }
}

Programmer::~Programmer() {
    cout << "Programmer Destructor invoked" << endl;
    delete[] tasks;
}

ostream& operator<<(ostream& output, const Programmer& programmer) {
    output << static_cast<const Employee&>(programmer);  // Use base class operator<< directly
    output << "Tasks: ";
    for (int i = 0; i < programmer.taskCount; ++i) {
        output << programmer.tasks[i] << ", ";
    }
    cout << endl;
}
