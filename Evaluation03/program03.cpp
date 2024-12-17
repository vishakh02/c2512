//1.3 Programmer { string* tasks; int taskCount; } : Employee | tasks is the multiple tasks - array of task 
        // swap function overloadable
        // here, create the dynamic programmer object assigned to employee pointer.
        // field 'tasks' is dynamic memoery.
        // operator << overloadable, swap is overridden.


#include <iostream>
#include <cstring>
#include <utility>

using namespace std;

// Employee class
class Employee 
{
    private:
        int* id;
        int* age;     
        char* name;
    
    public:
        // Constructor
        Employee(int id, int age, const char* name) 
        {
            cout << "\n" << "Argument Constructor"<<endl;
            this->id = new int(id);                  
            this->age = new int(age);   
            this->name = new char[strlen(name) + 1]; 
            strcpy(this->name, name);
        }
    
        // Destructor
        virtual ~Employee() 
        {
            cout << "Destructor called"<<endl;
            delete id;   
            delete age;  
            delete[] name; 
        }
    
        // Move Constructor
        Employee(Employee&& other) noexcept 
            : id(other.id), age(other.age), name(other.name) 
        {
            cout << "\n" << "MoveConstructor"<<endl;
            other.id = nullptr;
            other.age = nullptr;
            other.name = nullptr; 
        }
    
        // Move Assignment Operator
        Employee& operator=(Employee&& other) noexcept
        {
            if (this != &other) 
            {
                delete id;
                delete age;
                delete[] name;
    
                id = other.id;
                age = other.age;
                name = other.name;
    
                other.id = nullptr;
                other.age = nullptr;
                other.name = nullptr;
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
    out << *employee.id << " " << *employee.age << " " << employee.name;
    return out;
}

class Programmer : public Employee 
{
    private:
        string* tasks; 
        int taskCount; 
    
    public:
        // Constructor
        Programmer(int id, int age, const char* name, int taskCount, const string* tasks)
            : Employee(id, age, name), taskCount(taskCount) 
        {
            cout << "Programmer Arg Constructor"<<endl;
            this->tasks = new string[taskCount];
            for (int i = 0; i < taskCount; ++i) 
            {
                this->tasks[i] = tasks[i];
            }
        }
    
        // Destructor
        ~Programmer() 
        {
            cout << "\n" << "Programmer Destructorr"<<endl;
            delete[] tasks; 
        }
    
        // Move Constructor
        Programmer(Programmer&& other) noexcept
            : Employee(std::move(other)), taskCount(other.taskCount), tasks(other.tasks) 
        {
            cout << "Programmer MoveConstructor"<<endl;
            other.tasks = nullptr; 
        }
    
        // Move Assignment Operator
        Programmer& operator=(Programmer&& other) noexcept 
        {
            if (this != &other) 
            {
                delete[] tasks;
    
                Employee::operator=(std::move(other));
    
                tasks = other.tasks;
                
                taskCount = other.taskCount;
    
                other.tasks = nullptr;
            }
            return *this;
        }
    
        // Swap function using Move Constructor
        void swap(Programmer& other) 
        {
            Programmer temp(std::move(other)); // Move other into temp
            other = std::move(*this);          // Move this into other
            *this = std::move(temp);           // Move temp back into this
        }
    
        friend ostream& operator<<(ostream& os, const Programmer& prog);
};

ostream& operator<<(ostream& out, const Programmer& programmer) 
{
    out << static_cast<const Employee&>(programmer) << " Tasks: ";
    for (int i = 0; i < programmer.taskCount; ++i)
    {
        out << programmer.tasks[i] << (i < programmer.taskCount - 1 ? ", " : "");
    }
    return out;
}

int main() 
{
    const string tasks1[] = {"Task01", "Task02", "Task03"};
    Programmer p1(101, 23, "Athira", 3, tasks1);

    const string tasks2[] = {"Task_A", "Task_B", "Task_C"};
    Programmer p2(102, 22, "Bhagya", 3, tasks2);

    cout << " \n Before swapping:" << endl;
    cout << p1 << endl; 
    cout << p2 << endl; 

    p1.swap(p2);

    cout << "\nAfter swapping:" << endl;
    cout << p1 << endl; 
    cout << p2 << endl; 

    return 0;
}
