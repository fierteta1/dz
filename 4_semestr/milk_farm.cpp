#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Базовый класс Animal
class Animal {
protected:
    string name;
    string gender;

public:
    Animal(const string& name, const string& gender)
        : name(name), gender(gender) {}

    virtual ~Animal() {}

    virtual void produceSound() const = 0;
};

// Класс Cow, наследуется от Animal
class Cow : public Animal {
public:
    Cow(const string& name, const string& gender)
        : Animal(name, gender) {}

    void produceSound() const override {
        cout << "Мууу!" << endl;
    }
};

// Класс Goat, наследуется от Animal
class Goat : public Animal {
public:
    Goat(const string& name, const string& gender)
        : Animal(name, gender) {}

    void produceSound() const override {
        cout << "Меее!" << endl;
    }
};

// Базовый класс Product
class Product {
protected:
    string name;
    double price;

public:
    Product(const string& name, double price)
        : name(name), price(price) {}

    virtual ~Product() {}

    virtual string getDescription() const = 0;

    double getPrice() const {
        return price;
    }
};

// Класс Milk, наследуется от Product
class Milk : public Product {
public:
    Milk()
        : Product("Молоко", 100.0) {}

    string getDescription() const override {
        return name;
    }
};

// Класс Cheese, наследуется от Product
class Cheese : public Product {
public:
    Cheese()
        : Product("Сыр", 200.0) {}

    string getDescription() const override {
        return name;
    }
};

// Базовый класс AnimalFactory
class AnimalFactory {
public:
    virtual Animal* createAnimal(const string& name, const string& gender) const = 0;
};

// Фабрика для создания коров, наследуется от AnimalFactory
class CowFactory : public AnimalFactory {
public:
    Animal* createAnimal(const string& name, const string& gender) const override {
        return new Cow(name, gender);
    }
};

// Фабрика для создания коз, наследуется от AnimalFactory
class GoatFactory : public AnimalFactory {
public:
    Animal* createAnimal(const string& name, const string& gender) const override {
        return new Goat(name, gender);
    }
};

// Класс фермы
class Farm {
private:
    vector<Animal*> animals;           // Вектор для хранения животных
    vector<Product*> products;         // Вектор для хранения продукции
    AnimalFactory* animalFactory;      // Указатель на фабрику животных

    Farm() {
        animalFactory = nullptr;
    }

    ~Farm() {
        delete animalFactory;
        for (Animal* animal : animals) {
            delete animal;
        }
        for (Product* product : products) {
            delete product;
        }
    }

public:
    static Farm& getInstance() {
        static Farm instance;
        return instance;
    }

    // Установка фабрики животных
    void setAnimalFactory(AnimalFactory* factory) {
        animalFactory= factory;
    }

// Добавление животного на ферму
void addAnimal(const string& name, const string& gender) {
    if (animalFactory != nullptr) {
        Animal* animal = animalFactory->createAnimal(name, gender);
        animals.push_back(animal);
    }
}

// Производство продукции
void produceProducts() {
    for (Animal* animal : animals) {
        if (dynamic_cast<Cow*>(animal)) {
            products.push_back(new Milk());
        }
        else if (dynamic_cast<Goat*>(animal)) {
            products.push_back(new Cheese());
        }
    }
}

// Продажа продукции
void sellProducts() {
    double totalRevenue = 0.0;
    cout << "Продукция на ферме:" << endl;
    for (Product* product : products) {
        cout << product->getDescription() << " - " << product->getPrice() << " руб." << endl;
        totalRevenue += product->getPrice();
    }
    cout << "Общая выручка от продажи: " << totalRevenue << " руб." << endl;
}
};

int main() {
    Farm& farm = Farm::getInstance();
    farm.setAnimalFactory(new CowFactory());
    farm.addAnimal("Буренка", "женский");
    farm.addAnimal("Буренка2", "женский");
    farm.addAnimal("Буренка3", "женский");

    farm.setAnimalFactory(new GoatFactory());

    farm.addAnimal("Козлик", "мужской");
    farm.addAnimal("Козочка", "женский");

    farm.produceProducts();
    farm.sellProducts();

    return 0;
}
