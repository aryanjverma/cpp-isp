#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
class AbstractEmployee{
    virtual void AskForPromotion()=0;
};

class Employee:AbstractEmployee{
protected:
    string Name;
    string Company;
    int Age;
public:
    void setName(string name){
        Name = name;
    }
    string getName(){
        return Name;
    }
    void introduce(){
        cout << "I am " << Name << ", I work at " << Company << ", I am " << to_string(Age) << " years old.\n";
    }
    Employee(string name, string company, int age){
        Name = name;
        Company = company;
        Age = age;
    }
    void AskForPromotion(){
        if(Age > 30){
            cout << Name << " got promoted\n";
        }
        else{
            cout << Name << " got denied\n";
        }
    }
    virtual void work(){
        cout << Name << " works\n";
    }
};
class Developer:public Employee {
public:
    string FavoriteLanguage;
    Developer(string name, string company, int age, string favoriteLanguage)
        :Employee(name, company, age)
    {
        FavoriteLanguage = favoriteLanguage;
    }
    void FixBug(){
        cout << getName() << " is fixing bug using " << FavoriteLanguage;
    }
    void work(){
        cout << Name << " writes code\n";
    }
};
int main(){
    Employee employee1("John", "Apple", 40);
    employee1.introduce();
    employee1.AskForPromotion();
    Developer d("bob","Apple",50,"c++");
    
    Employee *e1 = &d;
    return 0;
}