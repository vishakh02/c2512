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










