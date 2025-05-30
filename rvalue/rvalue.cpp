#include <cstring>
#include <iostream>
#include <string>

class MyString {
public:
    char* data;
    size_t length;

    // Constructor
    MyString(const char* str = "") {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
        std::cout << "Constructor called for: " << data << std::endl;
    }

    // Copy constructor
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);
        std::cout << "Copy constructor called for: " << data << std::endl;
    }

    // Move constructor
    MyString(MyString&& other) noexcept {
        length = other.length;
        data = other.data;
        other.data = nullptr;
        other.length = 0;
        std::cout << "Move constructor called for: " << data << std::endl;
    }

    // Assignment operator
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            std::strcpy(data, other.data);
            std::cout << "Assignment operator called for: " << data << std::endl;
        }
        return *this;
    }

    // Move assignment operator
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = other.data;
            other.data = nullptr;
            other.length = 0;
            std::cout << "Move assignment operator called for: " << data << std::endl;
        }
        return *this;
    }

    // Destructor
    ~MyString() {
        delete[] data;
        std::cout << "Destructor called" << std::endl;
    }
};

MyString create_string() {
    return MyString("Hello, world!");
}

void process_string(MyString str) {
    std::cout << "Processing string: " << str.data << std::endl;
}

void process_string_rvalue(MyString&& str) {
    std::cout << "Processing rvalue string: " << str.data << std::endl;
}

int main() {
    MyString s1 = "Initial string"; // Constructor
    MyString s2 = s1; // Copy constructor
    MyString s3 = create_string(); // Move constructor

    s2 = s3; // Assignment operator
    s2 = create_string(); // Move assignment operator

    process_string(s1); // Copy constructor
    process_string(create_string()); // Move constructor

    process_string_rvalue(create_string()); // Move constructor

    return 0;
}
