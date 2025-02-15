#include <bits/stdc++.h>

using std::cout;
using std::vector;
using std::string;
using std::cerr;

//class Drink {
//public:
//    virtual void make() = 0;
//};
//
//class Tea : public Drink {
//public:
//    void make() override {
//        return;
//    }
//};

class Drink {
public:
    void make() {
        cout << "Drink" << "\n";
        return;
    };
};

class Tea : public Drink {
public:
    void make() {
        cout << "Tea\n";
        return;
    }
};

int main() {
    Drink drink;
    Tea tea;

    drink.make();
    tea.make();

    Drink *ptr1 = &drink;
    Drink *ptr2 = &tea;

    ptr1->make();
    ptr2->make();

    return 0;
}