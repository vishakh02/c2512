#include <iostream>

using namespace std;

class Employee{
    private:
     int Id;
     double age;
     string name;

    public:
     void swap(Employee& other);
     Employee(int Id, double age, string name);
     
     friend ostream& operator<<(ostream& output, const Employee& Employee);
};

ostream& operator<<(ostream& output, const Employee& employee){
    output << employee.Id << ", " << employee.age << ", " << employee.name;
    return output;
}

int main(){
    Employee e1(101,22,"Athira");
    Employee e2(102,23,"Bhagya");

    cout << "\n"; cout << "Original values: "<< endl; cout << "\n";
    cout << e1 << endl;
    cout << e2 << endl;

    e1.swap(e2);

    cout << "\n"; cout << "Swapped values: "<< endl; cout << "\n";
    cout << e1 << endl;
    cout << e2 << endl;

    return 0;
}

void Employee::swap(Employee& other){
    int temp = this->Id;
    this->Id = other.Id;
    other.Id = temp;

    double temp2 = this->age;
    this->age = other.age;
    other.age = temp2;

    string temp3 = this->name;
    this->name = other.name;
    other.name = temp3;
}

Employee::Employee(int Id, double age, string name): Id(Id), age(age), name(name){
        cout << "Constructer invoked" << endl;
}

