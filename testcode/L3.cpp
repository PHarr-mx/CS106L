#include <iostream>
#include <vector>

void test1() {
    int x = 3.14;
//    int y{3.14};
};

void test2() {
    std::vector<int> a;
    a.push_back(0);
    a.push_back(1);
    std::cout << a.at(1) << "\n";
//    std::cout << a.at(2) << "\n"; // 会检查越界
    std::cout << a[2] << "\n";


}

void test3(){
    std::vector<int> vec;
    std::vector<int>::iterator it = vec.begin(); // 创建迭代器
    std::cout << *it << std::endl; // 通过解引用获得迭代器原本的值
    ++ it; // 移动迭代器
    if(it == vec.end()) return; // 比较迭代器
}

int main(){

    return 0;
}