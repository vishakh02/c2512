Day 35


#include <iostream>

class Rectangle {
 
private:
 int* length;
 int* breadth;

public:
Rectangle():Rectangle(0,0) {}; // no arg constructor

Rectangle( int p_length, int p_breadth) {
	length = new int(p_length);
	breadth = new int(p_breadth);
}

Rectangle( Rectangle& other) {
	this->length =new int(*other.length);
	this->breadth = other.breadth;
}



Rectangle( Rectangle&& other) {
	this->length = other.length;
	this->breadth = other.breadth;
	other.length = nullptr;
	other,breadth = nullptr;
}

Rectangele& operator=(const Rectangle& other) {
	this->length = other.length;
	this->breadth = other.breadth;
	return *this;
}

Rectangele& operator=(const Rectangle&& other) {
	this->length = other.length;
	this->breadth = other.breadth;
	other.length = nullptr;
	other,breadth = nullptr;
	return *this;
}

~Rectangle() {
     if (this->length != nullptr){
     delete length;
    }
     if (this->breadth != nullptr){
     delete breadth;
    }
	
}
 
void display() {
	std::cout << "Length = " << length << ", Breadth = " << breadth;
}
};



int main() {
Rectangle obj1;
Rectangle obj2(2,4);
Rectangle obj3(obj2);

Rectangle obj4(1,2);
Rectangle obj5(std::move(obj4));

Rectangle obj6;
obj6 = obj2;     // copy assignment

Rectangle obj7(5,4);
Rectangle obj8;
obj8 = move(obj7); // move assignment

}









--±--++++++



	#include <iostream>
#include <memory> // For std::unique_ptr

class Rectangle {
private:
    std::unique_ptr<int> length;
    std::unique_ptr<int> breadth;

public:
    // Default constructor
    Rectangle() : Rectangle(0, 0) {}

    // Parameterized constructor
    Rectangle(int p_length, int p_breadth)
        : length(std::make_unique<int>(p_length)), breadth(std::make_unique<int>(p_breadth)) {}

    // Copy constructor (deleted, as unique_ptr cannot be copied)
    Rectangle(const Rectangle& other) = delete;

    // Move constructor
    Rectangle(Rectangle&& other) noexcept
        : length(std::move(other.length)), breadth(std::move(other.breadth)) {}

    // Copy assignment operator (deleted, as unique_ptr cannot be copied)
    Rectangle& operator=(const Rectangle& other) = delete;

    // Move assignment operator
    Rectangle& operator=(Rectangle&& other) noexcept {
        if (this != &other) {
            length = std::move(other.length);
            breadth = std::move(other.breadth);
        }
        return *this;
    }

    // Destructor (unique_ptr automatically handles memory cleanup)
    ~Rectangle() = default;

    // Display function
    void display() const {
        std::cout << "Length = " << (length ? *length : 0)
                  << ", Breadth = " << (breadth ? *breadth : 0) << '\n';
    }
};

int main() {
    Rectangle obj1; // Default constructor
    obj1.display();

    Rectangle obj2(2, 4); // Parameterized constructor
    obj2.display();

    Rectangle obj4(1, 2);
    Rectangle obj5(std::move(obj4)); // Move constructor
    obj5.display();

    Rectangle obj7(5, 4);
    Rectangle obj8;
    obj8 = std::move(obj7); // Move assignment operator
    obj8.display();

    return 0;
}










