#include <iostream>
#include <string>

using namespace std;

class Animal {
    private:
        string name;
        unsigned int age;
    public:
        Animal (string name, unsigned int age) {
            this->name = name;
            this->age = age;
        }
        
        virtual void MakeSound() = 0;
        
        string getName() const {
            return name;
        }
        
        unsigned int getAge() const {
            return age;
        }
        
};

class Cow : public Animal {
    private:
        unsigned int milk_ready;
        double avg_milk_yield;
    public:
        Cow(string name, unsigned int age, double avg_milk_yield, unsigned int milk_ready = 0)
            : Animal(name, age) {
                this->avg_milk_yield = avg_milk_yield;
                this->milk_ready = milk_ready;
        }

        virtual void MakeSound() {
            cout << "Mu mu mu" << endl;
        } 
        
        bool readyToMilk() const {
            return (milk_ready == 100) ? true : false;
        }

        double milk() {
            if (this->readyToMilk()) {
                milk_ready = 0;
                return avg_milk_yield;
            } else {
                cout << "Cow is not ready to milk" << endl;
                return 0;
            }
        }
        
        double getAvgMilkYield() const {
            return avg_milk_yield;
        }

        unsigned int getReadyMilk() const {
            return milk_ready;
        }

int main(void)
{
    Cow danusha("Danusha", 7, true, 30, 100);
    cout << dunusha.readyToMilk() << endl;
    double milk = dunusha.milk();
    dunusha.MakeSound();
}
