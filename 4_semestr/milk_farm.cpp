#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Базовый класс для продуктов
class Product {
protected:
    string name;     // Название продукта
    double price;    // Цена продукта

public:
    Product(const string& name, double price)
        : name(name), price(price) {}

    double getPrice() const {
        return price;
    }

    // Абстрактный метод для получения описания продукта
    virtual string getDescription() const = 0;
};

// Класс для молока, наследуется от Product
class Milk : public Product {
public:
    Milk(double price)
        : Product("Молоко", price) {}

    string getDescription() const override {
        return "Бутылка молока";
    }
};

// Класс для сыра, наследуется от Product
class Cheese : public Product {
public:
    Cheese(double price)
        : Product("Сыр", price) {}

    string getDescription() const override {
        return "Кусок сыра";
    }
};

// Базовый класс для животных
class Animal {
protected:
    string name;     // Имя животного
    string gender;   // Пол животного

public:
    Animal(const string& name, const string& gender)
        : name(name), gender(gender) {}

    // Абстрактный метод для издания звука
    virtual void produceSound() const = 0;

    // Абстрактный метод для производства продукции
    virtual void produceProduct(vector<Product*>& products) const = 0;

    // Абстрактный метод для смерти животного
    virtual void die() const = 0;

    string getGender() const {
        return gender;
    }
};

// Класс для коровы, наследуется от Animal
class Cow : public Animal {
public:
    Cow(const string& name, const string& gender)
        : Animal(name, gender) {}

    void produceSound() const override {
        cout << "Мууу!" << endl;
    }

    void produceProduct(vector<Product*>& products) const override {
        double milkPrice = 2.5;
        if (gender == "женский") {
            products.push_back(new Milk(milkPrice));
            products.push_back(new Cheese(10.0));
        }
    }

    void die() const override {
        cout << "Корова " << name << " умерла." << endl;
        delete this;
    }
};

// Класс для козы, наследуется от Animal
class Goat : public Animal {
public:
    Goat(const string& name, const string& gender)
        : Animal(name, gender) {}

    void produceSound() const override {
        cout << "Меее!" << endl;
    }

    void produceProduct(vector<Product*>& products) const override {
        double milkPrice = 3.0;
        if (gender == "женский") {
            products.push_back(new Milk(milkPrice));
            products.push_back(new Cheese(8.0));
        }
    }

    void die() const override {
        cout << "Коза " << name << " умерла." << endl;
        delete this;
    }
};

// Базовый класс для фабрик животных
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
    AnimalFactory* animalFactory;      // Указатель на фабрику животных

    Farm() {
        animalFactory = nullptr;
    }

    ~Farm() {
        delete animalFactory;
        for (Animal* animal : animals) {
            delete animal;
        }
    }

public:
    static Farm& getInstance() {
        static Farm instance;
        return instance;
    }

    // Установка фабрики животных
    void setAnimalFactory(AnimalFactory* factory) {
        animalFactory = factory;
    }

    // Добавление животного на ферму
    Animal* addAnimal(const string& name, const string& gender) {
        if (animalFactory != nullptr) {
            Animal* newAnimal = animalFactory->createAnimal(name, gender);
            animals.push_back(newAnimal);
            return newAnimal;
        }
        return nullptr;
    }

    // Продажа продукции
    void sellProducts() const {
        vector<Product*> products;

        for (const Animal* animal : animals) {
            animal->produceProduct(products);
        }

        double totalIncome = 0.0;

        cout << "Продукция, доступная для продажи:" << endl;
        for (const Product* product : products) {
            cout << product->getDescription() << " - " << product->getPrice() << " рублей." << endl;
            totalIncome += product->getPrice();
        }

        cout << "Общая прибыль от продажи: " << totalIncome << " рублей." << endl;

        for (Product* product : products) {
            delete product;
        }

        products.clear();
    }

    // Симуляция дня на ферме
    void simulateDay() const {
        for (const Animal* animal : animals) {
            animal->produceSound();
        }
    }

    // Завершение дня на ферме
    void endDay() const {
        for (const Animal* animal : animals) {
            animal->die();
        }
        animals.clear();
    }
};

// Класс CowFactory, который реализует интерфейс IAnimalFactory и создает объекты Cow
class CowFactory : public IAnimalFactory {
public:
    // Реализация метода создания животных
    IAnimal* createAnimal(Gender gender) override {
        return new Cow(gender);
    }
};

// Класс GoatFactory, который реализует интерфейс IAnimalFactory и создает объекты Goat
class GoatFactory : public IAnimalFactory {
public:
    // Реализация метода создания животных
    IAnimal* createAnimal(Gender gender) override {
        return new Goat(gender);
    }
};

int main() {
    // Создаем объект-синглтон фермы
    Farm& farm = Farm::getInstance();

    // Создаем объекты-фабрики для коров и козлов
    CowFactory cowFactory;
    GoatFactory goatFactory;

    // Добавляем на ферму несколько коров и козлов, используя фабричный метод
    farm.addAnimal(cowFactory.createAnimal(Gender::Male));
    farm.addAnimal(cowFactory.createAnimal(Gender::Female));
    farm.addAnimal(cowFactory.createAnimal(Gender::Female));
    farm.addAnimal(goatFactory.createAnimal(Gender::Male));
    farm.addAnimal(goatFactory.createAnimal(Gender::Female));

    // Выводим информацию о всех животных на ферме
    farm.printAnimalList();

    // Производим продукцию и выводим информацию о собранном молоке и сыре
    farm.produceProducts();

    // Продаем молоко и сыр и выводим информацию о выручке
    farm.sellProducts();

    // Убираем животных с фермы, чтобы они не занимали место и ресурсы
    farm.removeDeadAnimals();

    // Выводим информацию о всех животных на ферме после уборки мертвых
    farm.printAnimalList();

    return 0;
}
