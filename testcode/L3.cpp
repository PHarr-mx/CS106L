#include <iostream>
#include <vector>

void test1() {
    int x = 3.14;
//    int y{3.14};
};

void test2() {

}


int main(){
    std::vector<int> a;
    a.push_back(0);
    a.push_back(1);
    std::cout << a.at(1) << "\n";
//    std::cout << a.at(2) << "\n"; // 会检查越界
    std::cout << a[2] << "\n";
    return 0;
}