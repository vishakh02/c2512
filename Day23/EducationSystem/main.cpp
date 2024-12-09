#include <iostream>

using namespace::std;

class Person{
    private:
    string name;
    public:
    void introduce(){
	    cout << "Hi. I am " << name << endl;
    }
    //constructor
    Person(string name):name(name){
        cout << "Constructor of Person invoked." << endl;
    }
    //destructor
    ~Person(){
        cout << "Destructor of Person invoked." << endl;
    }
};

class Teacher : virtual public Person{
	private:
        string subject;
	public:
	void teach(){
		cout << "I am teaching " << subject << endl;
	}
	//constructor
	Teacher(string subject, string name): subject(subject), Person(name){
		cout << "Constructor of Teacher invoked" << endl;
	}
	//destructor
	~Teacher(){
		cout << "Destructor of Teacher invoked" << endl;
	}
};

class Student : virtual public Person{
	private:
        string grade;
	public:
	void study(){
		cout << "I got " << grade << endl;
	}
	//constructor
	Student(string grade, string name): Person(name), grade(grade){
		cout << "Constructor of Student invoked" << endl;
	}
	//destructor
	~Student(){
		cout << "Destructor of student invoked" << endl;
	}
};

class TeachingAssistant : public Teacher, public Student{
	private:
        int hours;
	public:
	void assist(){
		cout << "Hi I am assisting for " << hours << " hours" <<  endl;
	}
	//constructor
	TeachingAssistant(string name, string subject, string grade, int hours):Person(name), Teacher(subject,name),Student(grade,name), hours(hours){
		cout << "Constructor of TeachingAssistant invoked" << endl;
	}
	//destructor
	~TeachingAssistant(){
		cout << "Destructor of TeachingAssistant invoked" << endl;
	}
};

int main(){
    static  TeachingAssistant ta("john", "Mathematics", "A+", 5);
    ta.introduce();
    ta.teach();
    ta.study();
    ta.assist();

    return 0;
}
