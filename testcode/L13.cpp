#include <bits/stdc++.h>

using std::cout;
using std::vector;
using std::string;
using std::cerr;

class VecStr {
private:
    std::vector<std::string> data;
public:
    VecStr(VecStr &&other);

    VecStr &operator=(VecStr &&other);
};

VecStr::VecStr(VecStr &&other) : data(std::move(other)) {}

VecStr &VecStr::operator=(VecStr &&other) {
    if (this != &other) {
        data = std::move(other.data);
    }
    return *this;
}


class String {
private:
    char *data;
public:
    // 拷贝构造函数（初始化新对象）
    String(const String &other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }

    // 拷贝赋值操作符（修改已存在对象）
    String &operator=(const String &other) {
        if (this != &other) { // 处理自赋值
            delete[] data;    // 释放旧资源
            data = new char[strlen(other.data) + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    String(String &&other);

    String &operator=(String &&other);

    ~String() { delete[] data; } // 析构函数
};

template<typename T>
void swap(T &a, T &b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

int main() {
    int x = 12, y = 8;
    cout << "x = " << x << ", y = " << y << "\n";
    swap(x, y);
    cout << "x = " << x << ", y = " << y << "\n";

    return 0;
}