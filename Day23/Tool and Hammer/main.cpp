single inheritance with dynamic binding
=============================================================
one two dynamic/static objects of the Derived class 
pointed by base class pointer
and call all behaviours using Base class pointer 

Override Base Class behaviors to derived class.
Define constructors and destructors for all the classes 
Define all the fields as dynamic fields in each class 

If you use string, use c-string (don't use string class)
        ie. dynamic c-string [dynamic char array]
============================================================= 
Example 5: `Tool and Hammer`
- `Base Class:` `Tool`  
  - Fields: `toolName`, `weight`  
  - Pure Virtual Behaviors: `use()`, `repair()`  
- `Derived Class:` `Hammer`  
  - Fields: `handleMaterial`  
============================================================= 

#include <iostream>
#include <cstring>

using namespace std;

// Base class: Tool
class Tool {
protected:
    char* toolName;
    double weight;

public:
    // Constructor
    Tool(const char* name, double wgt) {
        toolName = new char[strlen(name) + 1];
        strcpy(toolName, name);
        weight = wgt;
        cout << "Tool constructor called.." << endl;
    }

    // Destructor
    virtual ~Tool() {
        cout << "Tool destructor called.." << endl;
        delete[] toolName;
    }

    // Pure virtual behaviors
    virtual void use() = 0;
    virtual void repair() = 0;
};

// Derived class: Hammer
class Hammer : public Tool {
protected:
    char* handleMaterial;

public:
    // Constructor
    Hammer(const char* name, double wgt, const char* material)
        : Tool(name, wgt) {
        handleMaterial = new char[strlen(material) + 1];
        strcpy(handleMaterial, material);
        cout << "Hammer constructor called.." << endl;
    }

    // Destructor
    ~Hammer() {
        cout << "Hammer destructor called.." << endl;
        delete[] handleMaterial;
    }

    // Override behaviors
    void use() override {
        cout << "Using the hammer: " << toolName 
             << " weighing " << weight << "kg, with a handle made of "
             << handleMaterial << "." << endl;
    }

    void repair() override {
        cout << "Repairing with the hammer: " << toolName 
             << " weighing " << weight << "kg, with a handle made of "
             << handleMaterial << "." << endl;
    }
};

int main() {
    // Create dynamic objects of Hammer pointed by Tool pointers
    Tool* tool1 = new Hammer("Claw Hammer", 1.5, "Wood");
    Tool* tool2 = new Hammer("Sledge Hammer", 4.0, "Steel");

    cout << "=== Tool 1 ===" << endl;
    tool1->use();
    tool1->repair();

    cout << "=== Tool 2 ===" << endl;
    tool2->use();
    tool2->repair();

    // Cleanup
    delete tool1;
    delete tool2;

    return 0;
}
