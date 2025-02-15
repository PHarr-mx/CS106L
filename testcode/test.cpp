#include <iostream>
#include <string>

using std::cin;
using std::cout;

//template<typename T>
//T foo(T a) {
//    return ++a;
//}

template<typename T>
concept integral = std::is_integral<T>::value;

template<typename T>
concept signed_integral = integral<T> and std::is_signed<T>::value;

template<typename T>
concept unsigned_integral = integral<T> && std::is_unsigned_v<T>;

template<integral T>
T foo1(T a) {
    return ++a;
}

integral auto foo2(integral auto a) {
    return ++a;
}

template<typename T>
T foo3(T a) requires integral<T> {
    return ++a;
}

template<typename T>
requires integral<T>
T foo4(T a) {
    return ++a;
}

template<typename T>
concept AddAble = requires(T a, T b) { a + b; }; // T 支持加法

template<typename T, typename U>
concept check1 = requires(T a, U b) {{ a + b }; { b + a }; }; // T U 支持相加

template<typename T>
concept check2 = requires(T a){
    { *a }; // *a 有意义
    { a + 1 } -> std::same_as<T>; // a + 1 有意义，且相加后类型不变。
    { std::same_as<decltype((a * a)), T> }; //
};


int main() {
    std::string a = "123";
//    foo(a);
    test b;
    b + b;
    f(b);
}