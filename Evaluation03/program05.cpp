#include <iostream>
#include <cstring>
#include <utility>
#include <memory>  
#include <vector>  

using namespace std;

// Employee class
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
            this->id = make_unique<int>(id);
            this->age = make_unique<int>(age);
            this->name = make_unique<char[]>(strlen(name) + 1);
            strcpy(this->name.get(), name);
        }
    
        // Default constructor
        Employee() : id(nullptr), age(nullptr), name(nullptr) {}
    
        // Move Constructor
        Employee(Employee&& other) noexcept
            : id(std::move(other.id)), age(std::move(other.age)), name(std::move(other.name)) {}
    
        // Move Assignment Operator
        Employee& operator=(Employee&& other) noexcept 
        {
            if (this != &other) 
            {
                id = std::move(other.id);
                age = std::move(other.age);
                name = std::move(other.name);
            }
            return *this;
        }
    
        friend ostream& operator<<(ostream& out, const Employee& employee);
 
        friend istream& operator>>(istream& input, Employee& employee);
    
        int getId() const { return *id; }
        int getAge() const { return *age; }
        const char* getName() const { return name.get(); }
};

ostream& operator<<(ostream& out, const Employee& employee) 
{
    out << *employee.id << " " << *employee.age << " " << employee.name.get();
    return out;
}

istream& operator>>(istream& input, Employee& employee)
{
    int id, age;
    string name;
    cout << "Enter Employee ID: ";
    input >> id;
    cout << "Enter Employee Age: ";
    input >> age;
    cout << "Enter Employee Name: ";
    input.ignore();  // To ignore the newline after age
    getline(input, name);
    
    employee.id = make_unique<int>(id);
    employee.age = make_unique<int>(age);
    employee.name = make_unique<char[]>(name.length() + 1);
    strcpy(employee.name.get(), name.c_str());
    return input;
}

class Programmer : public Employee
{
    private:
        vector<string> tasks; 
    
    public:
        // Default constructor
        Programmer() : Employee(), tasks() {}
    
        // Constructor
        Programmer(int id, int age, const char* name, int taskCount, const string* tasksArray)
            : Employee(id, age, name) 
        {
            tasks.reserve(taskCount);
            for (int i = 0; i < taskCount; ++i)
            {
                tasks.push_back(tasksArray[i]); 
            }
        }
    
        // Move Constructor
        Programmer(Programmer&& other) noexcept
            : Employee(std::move(other)), tasks(std::move(other.tasks)) {}
    
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
    
        // << operator to display Programmer details
        friend ostream& operator<<(ostream& out, const Programmer& programmer);
    
        // >> operator to read Programmer details
        friend istream& operator>>(istream& input, Programmer& programmer);
};

ostream& operator<<(ostream& out, const Programmer& programmer) 
{
    out << static_cast<const Employee&>(programmer) << "   Tasks: ";
    
    if (programmer.tasks.empty()) 
    {
        out << "No tasks available.";  // Debugging: Add check for empty task vector
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

istream& operator>>(istream& input, Programmer& programmer)
{
    Employee tempEmp;
    input >> tempEmp;  // Read the base Employee data
    
    int taskCount;
    cout << "Enter number of tasks: ";
    input >> taskCount;

    string* tasksArray = new string[taskCount];
    cout << "Enter tasks:" << endl;
    for (int i = 0; i < taskCount; ++i) 
    {
        input >> ws;  // to discard any leading whitespace (important for getline)
        getline(input, tasksArray[i]);
    }

    programmer = Programmer(tempEmp.getId(), tempEmp.getAge(), tempEmp.getName(), taskCount, tasksArray);

    delete[] tasksArray;  // Clean up the dynamically allocated array
    return input;
}

int main() 
{
    int N;
    cout << "Enter the number of programmers: ";
    cin >> N;

    vector<Programmer> programmers(N);

    // Read programmers from the user input
    for (int i = 0; i < N; ++i) 
    {
        cout << "Enter details for Programmer " << (i + 1) << endl;
        cin >> programmers[i];  // Overloaded >> operator
    }

    // Display all programmers
    cout << "\nProgrammer details:" << endl;
    for (int i = 0; i < N; ++i) 
    {
        cout << programmers[i] << endl;
    }

    return 0;
}
