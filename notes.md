# Streams

流可以认为是一个缓冲区，我们可以像先缓冲区中写入数据`<<`，也可以从缓冲区中读取数据`>>`。

## String Streams

```cpp
# include <bits/stdc++.h>

using namespace std;


int main(){
    ostringstream oss("Ito-En Green Tes");
    cout << oss.str() << endl; // 对于一个流，我们可以用 str() 获得缓存区中的内容

    oss << 16.9 << " Ounces"; 
    // 流中所有的所有数据都是以字符串形式存储，并且会在写入是把元素强制类型转换为字符串
    cout << oss.str() << endl;
}
```

上述代码中，我们发现两次输出的结果是一样的。这是因为流默认在每次写入后会移动指针，而默认的指针一开始是在开头。

```cpp
# include <bits/stdc++.h>

using namespace std;


int main(){
//    ostringstream oss("Ito-En Green Tes", ostringstream::bin);
    ostringstream oss("Ito-En Green Tes", ostringstream::ate);
    cout << oss.str() << endl;

    oss << 16.9 << " Ounces";
    cout << oss.str() << endl;
}
```

这样写构造函数，流的初始指针就会是末尾。

下面我们考虑输入字符串流

```cpp
# include <bits/stdc++.h>

using namespace std;


int main(){
    istringstream iss("16.9 Ounces");
    double amount;
    string unit;
    iss >> amount >> unit;
    cout << amount / 2 << " " << unit << endl;
}
```

刚才我们说到了，流内全部是用字符串存储的，但是在读入的时候，流会跟读入变量的类型来进行自动的截断，并进行强制类型转换。

```cpp
# include <bits/stdc++.h>

using namespace std;


int main(){
    istringstream iss("16.9 Ounces");
    int amount;
    string unit;
    iss >> amount >> unit;
    cout << amount << " " << unit << endl;
}
```

同样的，对于这个代码，我们在读入`amount`时，读入`16`后就没有下一个是`.`，但是`.`是不会出现在 int 中的，因此就会截断，并且`unit`会从`.`开始继续向后读入。

```cpp
int stringToInter(const string &s) {
    istringstream iss(s);
    int result;
    iss >> result;
    return result;
}
```

通过字符串流实现了一个字符串转换为整形的函数。

## State Bits (状态位)

state bits 用来表示流的状态。主要包含以下几种：

1. good bit，表示流处于正常状态。
2. eof bit，表示流已经达到文件末尾。
3. fail bit，表示流操作失败，通常是由格式错误或类型不匹配引起。
4. bad bit，表示流处于严重的错误，通常是不可恢复的，如设备故障。

如果出现类型不匹配的情况，fail bit会被激活，同时指针会回退到操作之前。

我们可以用以下代码输出一个流的状态位。

```cpp
void printStateBits(const istream & iss) {
    cout << "State bits: ";
    cout << (iss.good() ? "G" : "-");
    cout << (iss.fail() ? "F" : "-");
    cout << (iss.eof() ? "E" : "-");
    cout << (iss.bad() ? "B" : "-");
    cout << "\n";
}
```

## Stream Buffer

在 C++ 中，**流缓冲区**（Stream Buffer）是输入输出（I/O）系统的重要组成部分，用于管理数据的缓存和传输。流缓冲区是 `std::streambuf` 类的实例，它充当流（如 `std::istream` 和 `std::ostream`）与底层数据源（如文件、内存或设备）之间的桥梁。

### 流缓冲区的作用：

1. **数据缓存**：
    - 流缓冲区用于临时存储数据，以减少频繁访问底层数据源（如文件或设备）的开销。
    - 例如，写入文件时，数据会先存储在缓冲区中，当缓冲区满或显式刷新时，数据才会写入文件。

2. **提高性能**：
    - 通过减少直接 I/O 操作的次数，流缓冲区可以显著提高程序的性能。

3. **数据格式化**：
    - 流缓冲区与流对象（如 `std::cin` 和 `std::cout`）配合，支持数据的格式化输入输出。

---

### 流缓冲区的类型：

1. **输入缓冲区**：
    - 用于从数据源（如文件或键盘）读取数据。
    - 例如，`std::cin` 使用输入缓冲区来存储从键盘输入的数据。

2. **输出缓冲区**：
    - 用于向数据目标（如文件或屏幕）写入数据。
    - 例如，`std::cout` 使用输出缓冲区来存储要显示的数据。

3. **双向缓冲区**：
    - 同时支持输入和输出操作。

---

### 流缓冲区的关键操作：

1. **刷新缓冲区（Flush）**：
    - 将缓冲区中的数据强制写入底层数据源。
    - 例如，`std::flush` 或 `std::endl` 可以刷新输出缓冲区。

2. **同步缓冲区（Sync）**：
    - 将输入缓冲区中的数据与底层数据源同步。
    - 例如，`std::cin.sync()` 可以清空输入缓冲区。

3. **设置缓冲区**：
    - 可以使用 `std::streambuf::pubsetbuf()` 自定义缓冲区的大小和位置。

4. **获取和设置缓冲区指针**：
    - 使用 `std::streambuf::pbase()`、`std::streambuf::pptr()` 和 `std::streambuf::epptr()` 可以访问输出缓冲区的指针。
    - 使用 `std::streambuf::eback()`、`std::streambuf::gptr()` 和 `std::streambuf::egptr()` 可以访问输入缓冲区的指针。

# Initialization(初始化)

 C++ 中初始化的方法有很多，课程中说有不少于26种。

## Uniform Initialization

C++11 引入了**统一初始化**（Uniform Initialization），也称为**列表初始化**（List Initialization），旨在提供一种更一致、更简洁的初始化语法。它可以用于初始化各种类型的对象，包括基本类型、数组、结构体、类对象等。

### 特点

1. **一致性**：无论是基本类型、数组、结构体还是类对象，都可以使用 `{}` 进行初始化。
2. **防止窄化转换**：使用 `{}` 初始化时，编译器会检查是否存在窄化转换（如将 `double` 转换为 `int`），如果存在则会报错。
3. **避免歧义**：可以避免 C++ 中最令人头疼的解析问题（Most Vexing Parse），即编译器将某些语法解析为函数声明而非对象初始化。

> **窄化转换**（Narrowing Conversion）是指在 C++ 中将一个值从一种类型转换为另一种类型时，如果目标类型的范围无法完全容纳源类型的值，导致数据丢失或精度降低的情况。窄化转换可能会导致意外的行为或错误，因此在某些情况下需要避免。
>
> 常见例子
>
> 1. 从浮点数到整数
> 2. 从大范围整数到小范围整数
> 3. 从无符号整数到有符号整数
> 4. 从高精度浮点数到低精度浮点数
>
> 窄化转换可能会导致以下问题：
>
> 1. **数据丢失**：例如，将浮点数转换为整数时，小数部分会被截断。
> 2. **未定义行为**：如果目标类型无法容纳源类型的值，结果可能是未定义的。
> 3. **隐藏的错误**：窄化转换可能不会导致编译错误，但会在运行时引发问题。
>
> 在 C++11 中，统一初始化（使用 `{}`）会禁止窄化转换。如果检测到窄化转换，编译器会报错。
>
> ```
> int x{3.14};  // 错误：从 double 到 int 的窄化转换
> int y = 3.14; // 合法：传统初始化允许窄化转换
> ```

# Sequence Containers

## `std::vector<T>`

```cpp
int main(){
    std::vector<int> a;
    a.push_back(0);
    a.push_back(1);
    std::cout << a.at(1) << "\n";
//    std::cout << a.at(2) << "\n"; // 会检查越界
    std::cout << a[2] << "\n";
    return 0;
}
```

这里如果`at`和`operator[]`都可以用来访问元素，但是`at`会检查是否越界，并且如果越界会抛出异常，但是`operator[]`并不会。

为什么 `operator[]` 不检查越界？

- **性能考虑**：`operator[]` 的设计目标是高效，避免额外的检查开销。
- **灵活性**：C++ 强调“零开销抽象”，允许程序员在需要时手动管理边界检查。
- **历史原因**：`operator[]` 的行为与 C 风格数组一致，保持了语言的连贯性。