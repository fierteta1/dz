#include <iostream>
#include <vector>
using namespace std;

// Определяем класс животного
class Animal {
protected:
    std::string name;
    int age;
    std::string breed;
    bool isPregnant;
public:
    Animal(std::string name, int age, std::string breed, bool isPregnant);
    void setName(std::string name);
    void setAge(int age);
    void setBreed(std::string breed);
    void setIsPregnant(bool isPregnant);
    std::string getName();
    int getAge();
    std::string getBreed();
    bool getIsPregnant();
    virtual void makeSound() = 0;
};

// Класс Cow, который наследуется от класса Animal

class Cow : public Animal {
protected:
    int milkProduction;
public:
    Cow(std::string name, int age, std::string breed, bool isPregnant, int milkProduction);
    void setMilkProduction(int milkProduction);
    int getMilkProduction();
    void makeSound() override;
};

// class Chicken : public Animal {
protected:
    int eggProduction;
public:
    Chicken(std::string name, int age, std::string breed, bool isPregnant, int eggProduction);
    void setEggProduction(int eggProduction);
    int getEggProduction();
    void makeSound() override;
};

// Класс Farm, который содержит списки животных и функции для добавления, удаления и управления животными и производством продуктов
class Farm {
protected:
    std::vector<Cow*> cows;
    std::vector<Chicken*> chickens;
    int milkProduction;
    int eggProduction;
public:
    Farm();
    void addCow(Cow* cow);
    void removeCow(Cow* cow);
    void addChicken(Chicken* chicken);
    void removeChicken(Chicken* chicken);
    void collectMilk();
    void collectEggs();
};
