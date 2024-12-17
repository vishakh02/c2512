#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class Employee
{
private:
    int *id;
    int *age;
    char *name;

public:
    Employee(int id, int age, const char *name);
    Employee(Employee &&other);
    Employee &operator=(Employee &&other);
    virtual ~Employee();
    virtual void swp(Employee &other);
    friend ostream &operator<<(ostream &out, const Employee &employee);
};

Employee::Employee(int id, int age, const char *name)
{
    this->id = new int{id};
    this->age = new int{age};
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

Employee::Employee(Employee &&other)
{
    this->id = other.id;
    this->age = other.age;
    this->name = other.name;
    other.id = nullptr;
    other.age = nullptr;
    other.name = nullptr;
}

Employee &Employee::operator=(Employee &&other)
{
    if (this != &other)
    {
        delete this->id;
        delete this->age;
        delete[] this->name;
        this->id = other.id;
        this->age = other.age;
        this->name = other.name;
        other.id = nullptr;
        other.age = nullptr;
        other.name = nullptr;
    }
    return *this;
}

Employee::~Employee()
{
    delete this->id;
    delete this->age;
    delete[] this->name;
}

void Employee::swp(Employee &other)
{
    Employee temp(move(*this));
    *this = move(other);
    other = move(temp);
}

ostream &operator<<(ostream &out, const Employee &employee)
{
    out << "ID: " << *(employee.id) << ", Name: " << employee.name << ", Age: " << *(employee.age);
    return out;
}

class Programmer : public Employee
{
private:
    string *tasks;
    int *taskCount;

public:
    Programmer(int v_id, int v_age, const char *v_name, string *v_tasks, int v_taskCount)
        : Employee(v_id, v_age, v_name)
    {
        this->taskCount = new int{v_taskCount};
        this->tasks = new string[v_taskCount];
        for (int i = 0; i < v_taskCount; i++)
        {
            this->tasks[i] = v_tasks[i];
        }
    }

    Programmer(Programmer &&other) : Employee(move(other))
    {
        this->taskCount = other.taskCount;
        this->tasks = other.tasks;
        other.taskCount = nullptr;
        other.tasks = nullptr;
    }

    Programmer &operator=(Programmer &&other)
    {
        if (this != &other)
        {
            Employee::operator=(move(other));
            delete this->taskCount;
            delete[] this->tasks;
            this->taskCount = other.taskCount;
            this->tasks = other.tasks;
            other.taskCount = nullptr;
            other.tasks = nullptr;
        }
        return *this;
    }

    ~Programmer()
    {
        delete taskCount;
        delete[] tasks;
    }

    void swp(Employee &other) override
    {
        if (Programmer *pOther = dynamic_cast<Programmer *>(&other))
        {
            Programmer temp(move(*this));
            *this = move(*pOther);
            *pOther = move(temp);
        }
        else
        {
            throw runtime_error("Incompatible types for swapping.");
        }
    }

    friend ostream &operator<<(ostream &out, const Programmer &programmer);
};

ostream &operator<<(ostream &out, const Programmer &programmer)
{
    out << static_cast<const Employee &>(programmer);
    out << ", Task Count: " << *(programmer.taskCount) << ", Tasks: [";
    for (int i = 0; i < *(programmer.taskCount); i++)
    {
        out << programmer.tasks[i];
        if (i < *(programmer.taskCount) - 1)
            out << ", ";
    }
    out << "]";
    return out;
}

int main()
{
    int N;
    cout << "Enter the number of programmers: ";
    cin >> N;

    Programmer **programmers = new Programmer *[N];

    for (int i = 0; i < N; ++i)
    {
        int id, age, taskCount;
        string name;

        cout << "\nEnter details for Programmer " << (i + 1) << ":\n";
        cout << "ID: ";
        cin >> id;
        cout << "Age: ";
        cin >> age;
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Number of tasks: ";
        cin >> taskCount;

        string *tasks = new string[taskCount];
        cout << "Enter the tasks:\n";
        cin.ignore();
        for (int j = 0; j < taskCount; ++j)
        {
            cout << "Task " << (j + 1) << ": ";
            getline(cin, tasks[j]);
        }

        programmers[i] = new Programmer(id, age, name.c_str(), tasks, taskCount);
        delete[] tasks;
    }

    cout << "\nDetails of all programmers:\n";
    for (int i = 0; i < N; ++i)
    {
        cout << *programmers[i] << endl;
    }

    for (int i = 0; i < N; ++i)
    {
        delete programmers[i];
    }
    delete[] programmers;

    return 0;
}