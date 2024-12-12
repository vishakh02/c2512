#include <iostream>
#include <string>

using namespace std;

// Base Class: Book
class Book {
protected:
    string title;

public:
    // Constructor
    Book(string bookTitle) : title(bookTitle) {
        cout << "Book Constructor: " << title << endl;
    }

    // Destructor
    virtual ~Book() {
        cout << "Book Destructor: " << title << endl;
    }

    // Base class behavior
    void read() {
        cout << "Reading the book: " << title << endl;
    }
};

// Derived Class: EBook
class EBook : public Book {
private:
    int fileSize; // in MB

public:
    // Constructor
    EBook(string bookTitle, int size) : Book(bookTitle), fileSize(size) {
        cout << "EBook Constructor: " << title << ", File size: " << fileSize << "MB" << endl;
    }

    // Destructor
    ~EBook() {
        cout << "EBook Destructor: " << title << endl;
    }

    // Derived class behavior
    void download() {
        cout << "Downloading the eBook: " << title << " (" << fileSize << "MB)" << endl;
    }
};

int main() {
    // Static Objects
    cout << "/Static Objects/" << endl;
    Book physicalBookStatic("The Great Gatsby");    // Base class static object
    EBook ebookStatic("1984", 5);                  // Derived class static object

    physicalBookStatic.read();                      // Base class behavior
    ebookStatic.read();                             // Inherited behavior
    ebookStatic.download();                         // Derived class behavior

    // Dynamic Objects
    cout << "/Dynamic Objects/" << endl;
    Book* physicalBookDynamic = new Book("To Kill a Mockingbird"); // Base class dynamic object
    Book* ebookDynamic = new EBook("Brave New World", 10);         // Derived class dynamic object

    physicalBookDynamic->read();                   // Call base class behavior
    ebookDynamic->read();                          // Call inherited behavior

    // Clean up dynamic objects
    delete physicalBookDynamic;
    delete ebookDynamic;

    return 0;
}
