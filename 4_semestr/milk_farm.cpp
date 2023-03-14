#include <iostream>
#include <vector>
using namespace std;

// Определяем класс животного
class Animal {
private:
    string name;
    int age;
public:
    Animal(string name, int age) {
        this->name = name;
        this->age = age;
    }

    // Методы получения и установки значений полей
    string getName() {
        return name;
    }

    int getAge() {
        return age;
    }
};
