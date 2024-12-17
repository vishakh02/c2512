#include <iostream>
#include <cstring>
#include <utility>
#include <memory> 
#include <vector> 

using namespace std;


class Employee 
{
    private:
        unique_ptr<int> id;
        unique_ptr<int> age;
        unique_ptr<char[]> name;
    
    public:
        // Constructor
        Employee(int id, int age, const char* name) 
        {
            cout << "\n" << "Employee Argument constructor called" << endl;
            this->id = make_unique<int>(id);
            this->age = make_unique<int>(age);
            this->name = make_unique<char[]>(strlen(name) + 1);
            strcpy(this->name.get(), name);
        }
    
        // Move Constructor
        Employee(Employee&& other) noexcept
            : id(std::move(other.id)), age(std::move(other.age)), name(std::move(other.name)) {
                cout << "\n" << "Employee Move constructor called" << endl;
            }
    
        // Move Assignment Operator
        Employee& operator=(Employee&& other) noexcept 
        {
            if (this != &other) {
                id = std::move(other.id);
                age = std::move(other.age);
                name = std::move(other.name);
            }
            return *this;
        }
    
        virtual void swap(Employee& other) 
        {
            Employee temp(std::move(other));
            other = std::move(*this);
            *this = std::move(temp);
        }
    
        friend ostream& operator<<(ostream& out, const Employee& employee);
};

ostream& operator<<(ostream& out, const Employee& employee) 
{
    out << *employee.id << " " << *employee.age << " " << employee.name.get();
    return out;
}

class Programmer : public Employee 
{
    private:
        vector<string> tasks; 
    
    public:
        // Constructor
        Programmer(int id, int age, const char* name, int taskCount, const string* tasksArray)
            : Employee(id, age, name) 
        {
            cout << "Programmer Argument constructor called" << endl;
            
            tasks.reserve(taskCount);
            for (int i = 0; i < taskCount; ++i) 
            {
                tasks.push_back(tasksArray[i]); 
            }
        }
    
        // Move Constructor
        Programmer(Programmer&& other) noexcept
            : Employee(std::move(other)), tasks(std::move(other.tasks)) {
                cout << "Programmer Move constructor called" << endl;
            }
    
        // Move Assignment Operator
        Programmer& operator=(Programmer&& other) noexcept 
        {
            if (this != &other) 
            {
                Employee::operator=(std::move(other));
                tasks = std::move(other.tasks);
            }
            return *this;
        }
    
        void swap(Programmer& other) 
        {
            Programmer temp(std::move(other)); 
            other = std::move(*this);          
            *this = std::move(temp);           
        }
    
        
        friend ostream& operator<<(ostream& out, const Programmer& programmer);
};

ostream& operator<<(ostream& out, const Programmer& programmer) 
{
    out << static_cast<const Employee&>(programmer) << ", Tasks: ";
    
    if (programmer.tasks.empty())
    {
        out << "No tasks available.";  
    } 
    else 
    {
        for (size_t i = 0; i < programmer.tasks.size(); ++i) 
        {
            out << programmer.tasks[i] << (i < programmer.tasks.size() - 1 ? ", " : "");
        }
    }

    return out;
}

int main() 
{
    const string tasks1[] = {"Code", "Debug", "Test"};
    Programmer p1(101, 23, "Athira", 3, tasks1);

    const string tasks2[] = {"Design", "Develop", "Deploy"};
    Programmer p2(102, 22, "Bhagya", 3, tasks2);
    
    const string tasks3[] = {"Research", "Implement", "Test"};
    unique_ptr<Employee> empPtr = make_unique<Programmer>(103, 21, "Aysha", 3, tasks3);

    cout << "\nBefore swapping:" << endl;
    cout << p1 << endl; 
    cout << p2 << endl; 

    p1.swap(p2);

    cout << "\nAfter swapping:" << endl;
    cout << p1 << endl; 
    cout << p2 << endl; 

     cout << "Dynamic Programmer assigned to Employee pointer:" << endl;
    Programmer* progPtr = dynamic_cast<Programmer*>(empPtr.get());
    if (progPtr) 
    {
        cout << *progPtr << endl; 
    } 
    else
    {
        cout << "Not a Programmer!" << endl;
    }
    return 0;
}
