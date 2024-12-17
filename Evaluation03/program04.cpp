#include <iostream>
#include <string>
#include <memory>
#include <cstring>

using namespace std;

class Employee
{
private:
    unique_ptr<int> id;
    unique_ptr<int> age;
    unique_ptr<char[]> name;

public:
    Employee(int id, int age, const char *name);
    Employee(Employee &&other) noexcept;
    Employee &operator=(Employee &&other) noexcept;
    virtual ~Employee() = default;
    virtual void swp(Employee &other);
    friend ostream &operator<<(ostream &out, const Employee &employee);
};

Employee::Employee(int id, int age, const char *name)
{
    this->id = make_unique<int>(id);
    this->age = make_unique<int>(age);
    this->name = make_unique<char[]>(strlen(name) + 1);
    strcpy(this->name.get(), name);
}

Employee::Employee(Employee &&other) noexcept
    : id(move(other.id)), age(move(other.age)), name(move(other.name))
{
}

Employee &Employee::operator=(Employee &&other) noexcept
{
    if (this != &other)
    {
        id = move(other.id);
        age = move(other.age);
        name = move(other.name);
    }
    return *this;
}

void Employee::swp(Employee &other)
{
    Employee temp(move(*this));
    *this = move(other);
    other = move(temp);
}

ostream &operator<<(ostream &out, const Employee &employee)
{
    out << "ID: " << *(employee.id) << ", Name: " << employee.name.get() << ", Age: " << *(employee.age);
    return out;
}

// Programmer class
class Programmer : public Employee
{
private:
    unique_ptr<string[]> tasks;
    unique_ptr<int> taskCount;

public:
    Programmer(int v_id, int v_age, const char *v_name, string *v_tasks, int v_taskCount);
    Programmer(Programmer &&other) noexcept;
    Programmer &operator=(Programmer &&other) noexcept;
    ~Programmer() = default;
    void swp(Employee &other) override;
    friend ostream &operator<<(ostream &out, const Programmer &programmer);
};

Programmer::Programmer(int v_id, int v_age, const char *v_name, string *v_tasks, int v_taskCount)
    : Employee(v_id, v_age, v_name)
{
    taskCount = make_unique<int>(v_taskCount);
    tasks = make_unique<string[]>(v_taskCount);
    for (int i = 0; i < v_taskCount; i++)
    {
        tasks[i] = v_tasks[i];
    }
}

Programmer::Programmer(Programmer &&other) noexcept
    : Employee(move(other)), tasks(move(other.tasks)), taskCount(move(other.taskCount))
{
}

Programmer &Programmer::operator=(Programmer &&other) noexcept
{
    if (this != &other)
    {
        Employee::operator=(move(other));
        tasks = move(other.tasks);
        taskCount = move(other.taskCount);
    }
    return *this;
}

void Programmer::swp(Employee &other)
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

// Test the code
int main()
{
    string tasks1[] = {"Code", "Debug", "Test"};
    unique_ptr<Employee> p1 = make_unique<Programmer>(101, 22, "Athira", tasks1, 3);

    string tasks2[] = {"Design", "Implement"};
    unique_ptr<Employee> p2 = make_unique<Programmer>(102, 23, "Bhagya", tasks2, 2);

    cout << "Before Swap:" << endl;
    cout << *p1 << endl;
    cout << *p2 << endl;

    try
    {
        p1->swp(*p2);
    }
    catch (const runtime_error &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    cout << "\nAfter Swap:" << endl;
    cout << *p1 << endl;
    cout << *p2 << endl;

    return 0;
}