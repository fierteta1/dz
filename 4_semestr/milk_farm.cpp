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

    string getName() const {
        return name;
    }
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
        animalFactory = factory;
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

// Вывод информации о количестве животных на ферме
void printAnimalCount() const {
    cout << "На ферме сейчас живут " << animals.size() << " животных." << endl;
}

// Покупка животного
void buyAnimal() {
    string animalType;
    string name;
    string gender;
    
    cout << "Выберите тип животного (корова/коза): ";
    cin >> animalType;
    cout << "Введите имя животного: ";
    cin >> name;
    cout << "Введите пол животного (мужской/женский): ";
    cin >> gender;
    
    if (animalType == "корова") {
        setAnimalFactory(new CowFactory());
    }
    else if (animalType == "коза") {
        setAnimalFactory(new GoatFactory());
    }
    
    addAnimal(name, gender);
    cout << "Животное успешно куплено и добавлено на ферму." << endl;
}

// Продажа животного
void sellAnimal() {
    if (animals.empty()) {
        cout << "На ферме нет животных для продажи." << endl;
        return;
    }

    string name;
    cout << "Введите имя животного, которое хотите продать: ";
    cin >> name;

    for (size_t i = 0; i < animals.size(); i++) {
        if (animals[i]->getName() == name) {
            delete animals[i];
            animals.erase(animals.begin() + i);
            cout << "Животное успешно продано." << endl;
            return;
        }
    }

    cout << "Животное с таким именем не найдено на ферме." << endl;
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
int choice;
    do {
        cout << "Выберите действие: " << endl;
        cout << "1. Посмотреть сколько животных на ферме" << endl;
        cout << "2. Купить животное" << endl;
        cout << "3. Продать животное" << endl;
        cout << "4. Произвести продукцию" << endl;
        cout << "5. Продать продукцию" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
            case 1:
                farm.printAnimalCount();
                break;
            case 2:
                farm.buyAnimal();
                break;
            case 3:
                farm.sellAnimal();
                break;
            case 4:
                farm.produceProducts();
                cout << "Продукция успешно произведена." << endl;
                break;
            case 5:
                farm.sellProducts();
                break;
            case 0:
                cout << "Выход из программы." << endl;
                break;
            default:
                cout << "Неверный выбор. Попробуйте еще раз." << endl;
        }
    } while (choice != 0);

    return 0;
}
