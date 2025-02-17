# Design philosophy of C++

- Allow the programmer full control, responsibility, and choice if they want it.
    - **解释**: C++ 强调程序员对代码的完全控制权，允许程序员在需要时直接管理内存、硬件资源等底层细节。
    - **体现**:
        - **手动内存管理**: 通过 `new` 和 `delete` 直接控制内存分配和释放。
        - **指针和引用**: 提供指针和引用，允许直接操作内存地址。
        - **底层操作**: 支持位操作、内联汇编等底层特性。
        - **零开销抽象**: C++ 的高级特性（如模板、RAII）在运行时几乎没有额外开销。
- Express ideas and intent directly in code.
    - **解释**: C++ 鼓励代码清晰地表达设计意图，使代码易于理解和维护。
    - **体现**:
        - **强类型系统**: 通过类型系统明确表达数据的含义。
        - **面向对象编程**: 使用类、继承和多态来表达抽象和层次结构。
        - **模板和泛型编程**: 通过模板实现通用的、类型安全的代码。
        - **RAII**: 通过构造函数和析构函数明确资源管理的生命周期。
- Enforce safety at compile time whenever possible.
    - **解释**: C++ 强调在编译时尽可能捕获错误，而不是在运行时。
    - **体现**:
        - **类型检查**: 编译器在编译时检查类型错误。
        - **模板实例化**: 模板在编译时实例化，确保类型安全。
        - **`constexpr`**: 在编译时计算常量表达式，避免运行时错误。
        - **`static_assert`**: 在编译时断言条件是否满足。
- Do not waste time or space.
    - **解释**: C++ 追求高效，避免不必要的运行时开销和内存浪费。
    - **体现**:
        - **零开销抽象**: 高级特性（如模板、RAII）在运行时几乎没有额外开销。
        - **内联函数**: 通过 `inline` 关键字减少函数调用开销。
        - **移动语义**: 通过移动构造函数和移动赋值运算符避免不必要的拷贝。
        - **内存对齐**: 支持手动控制内存对齐，优化性能。
- Compartmentalize messy constructs.
    - **解释**: C++ 鼓励将复杂或容易出错的代码隔离到特定的模块或类中，以提高代码的可维护性和安全性。
    - **体现**:
        - **命名空间**: 使用命名空间隔离代码，避免命名冲突。
        - **类封装**: 将数据和操作封装在类中，隐藏实现细节。
        - **RAII**: 将资源管理封装在类的构造函数和析构函数中。
        - **模板特化**: 将特定类型的特殊处理隔离到模板特化中。

## 总结

1. **控制与选择**: 程序员可以完全控制代码的行为，同时享受高级抽象的便利。
2. **表达意图**: 代码清晰地表达设计意图，易于理解和维护。
3. **编译时安全**: 尽可能在编译时捕获错误，减少运行时问题。
4. **高效性**: 避免不必要的开销，追求时间和空间的最优利用。
5. **隔离复杂性**: 将复杂或容易出错的代码隔离到特定模块中，提高代码的可维护性。

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

C++中有五种Sequence Containers，分别是`std::vector<T>, std::deque<T>, std::list<T>, std::array<T>, std::forward_list<T>`

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

# Container Adaptors

在 C++ 标准库中，**容器适配器（Container Adaptors）** 是一种特殊的容器，它们基于现有的标准容器（如 `std::vector`、`std::deque` 或 `std::list`）实现，但提供了不同的接口和行为。容器适配器并不是独立的容器，而是对现有容器的封装，用于实现特定的数据结构（如栈、队列和优先队列）。

C++ 提供了三种主要的容器适配器：

---

### 1. **`std::stack`**
- **功能**: 实现栈（后进先出，LIFO）的数据结构。
- **默认底层容器**: `std::deque`。
- **支持的操作**:
  - `push()`: 将元素压入栈顶。
  - `pop()`: 弹出栈顶元素。
  - `top()`: 访问栈顶元素。
  - `empty()`: 检查栈是否为空。
  - `size()`: 返回栈中元素的数量。
- **示例**:
  ```cpp
  #include <iostream>
  #include <stack>
  
  int main() {
      std::stack<int> s;
      s.push(1);
      s.push(2);
      s.push(3);
  
      while (!s.empty()) {
          std::cout << s.top() << " ";  // 输出: 3 2 1
          s.pop();
      }
  }
  ```

---

### 2. **`std::queue`**
- **功能**: 实现队列（先进先出，FIFO）的数据结构。
- **默认底层容器**: `std::deque`。
- **支持的操作**:
  - `push()`: 将元素添加到队尾。
  - `pop()`: 移除队首元素。
  - `front()`: 访问队首元素。
  - `back()`: 访问队尾元素。
  - `empty()`: 检查队列是否为空。
  - `size()`: 返回队列中元素的数量。
- **示例**:
  ```cpp
  #include <iostream>
  #include <queue>
  
  int main() {
      std::queue<int> q;
      q.push(1);
      q.push(2);
      q.push(3);
  
      while (!q.empty()) {
          std::cout << q.front() << " ";  // 输出: 1 2 3
          q.pop();
      }
  }
  ```

---

### 3. **`std::priority_queue`**
- **功能**: 实现优先队列（元素按优先级排序，默认最大堆）。
- **默认底层容器**: `std::vector`。
- **支持的操作**:
  - `push()`: 将元素添加到优先队列。
  - `pop()`: 移除优先级最高的元素。
  - `top()`: 访问优先级最高的元素。
  - `empty()`: 检查优先队列是否为空。
  - `size()`: 返回优先队列中元素的数量。
- **自定义优先级**:
  可以通过提供自定义的比较函数来改变优先级规则。
- **示例**:
  ```cpp
  #include <iostream>
  #include <queue>
  
  int main() {
      std::priority_queue<int> pq;
      pq.push(3);
      pq.push(1);
      pq.push(4);
  
      while (!pq.empty()) {
          std::cout << pq.top() << " ";  // 输出: 4 3 1
          pq.pop();
      }
  }
  ```

  如果需要最小堆，可以这样定义：
  ```cpp
  std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
  ```

---

### 容器适配器的特点
1. **基于现有容器**:
   - 容器适配器并不是独立的容器，而是基于现有的标准容器（如 `std::deque`、`std::vector` 等）实现的。
   - 例如，`std::stack` 默认使用 `std::deque` 作为底层容器，但也可以指定其他容器（如 `std::vector` 或 `std::list`）。

2. **简化接口**:
   - 容器适配器提供了特定数据结构的接口（如栈的 `push` 和 `pop`），隐藏了底层容器的复杂性。

3. **灵活性**:
   - 可以通过模板参数指定底层容器类型。例如：
     ```cpp
     std::stack<int, std::vector<int>> s;  // 使用 std::vector 作为底层容器
     ```

---

### 容器适配器的底层容器
- **`std::stack`**:
  - 默认使用 `std::deque`，但也可以使用 `std::vector` 或 `std::list`。
- **`std::queue`**:
  - 默认使用 `std::deque`，但也可以使用 `std::list`。
- **`std::priority_queue`**:
  - 默认使用 `std::vector`，但也可以使用 `std::deque`。

---

### 总结
容器适配器是对现有标准容器的封装，提供了特定数据结构（如栈、队列和优先队列）的接口。它们的优点是：
- 简化了特定数据结构的实现。
- 基于现有的高效容器，性能有保障。
- 灵活支持自定义底层容器。

如果需要实现栈、队列或优先队列，优先考虑使用容器适配器，而不是手动实现。

# Associative Containers

C++ 标准库提供了几种关联容器（Associative Containers），这些容器基于键（key）来存储和访问元素。关联容器的主要特点是它们能够高效地根据键来查找、插入和删除元素。以下是 C++ 中常见的关联容器：

### 1. `std::set`
- **特点**: `std::set` 是一个有序的集合，其中的元素是唯一的（即不允许重复）。
- **底层实现**: 通常基于红黑树（一种平衡二叉搜索树）。
- **操作复杂度**: 插入、删除和查找操作的平均时间复杂度为 O(log n)。
- **示例**:
  ```cpp
  #include <iostream>
  #include <set>
  
  int main() {
      std::set<int> s;
      s.insert(3);
      s.insert(1);
      s.insert(4);
  
      for (int x : s) {
          std::cout << x << " ";  // 输出: 1 3 4
      }
  }
  ```

### 2. `std::multiset`
- **特点**: `std::multiset` 与 `std::set` 类似，但它允许存储重复的元素。
- **底层实现**: 同样基于红黑树。
- **操作复杂度**: 插入、删除和查找操作的平均时间复杂度为 O(log n)。
- **示例**:
  ```cpp
  #include <iostream>
  #include <set>
  
  int main() {
      std::multiset<int> ms;
      ms.insert(3);
      ms.insert(1);
      ms.insert(4);
      ms.insert(1);
  
      for (int x : ms) {
          std::cout << x << " ";  // 输出: 1 1 3 4
      }
  }
  ```

### 3. `std::map`
- **特点**: `std::map` 是一个有序的键值对容器，键是唯一的。
- **底层实现**: 通常基于红黑树。
- **操作复杂度**: 插入、删除和查找操作的平均时间复杂度为 O(log n)。
- **示例**:
  ```cpp
  #include <iostream>
  #include <map>
  
  int main() {
      std::map<std::string, int> m;
      m["apple"] = 5;
      m["banana"] = 3;
      m["orange"] = 8;
  
      for (const auto& pair : m) {
          std::cout << pair.first << ": " << pair.second << std::endl;
      }
      // 输出:
      // apple: 5
      // banana: 3
      // orange: 8
  }
  ```

### 4. `std::multimap`
- **特点**: `std::multimap` 与 `std::map` 类似，但它允许键重复。
- **底层实现**: 同样基于红黑树。
- **操作复杂度**: 插入、删除和查找操作的平均时间复杂度为 O(log n)。
- **示例**:
  ```cpp
  #include <iostream>
  #include <map>
  
  int main() {
      std::multimap<std::string, int> mm;
      mm.insert({"apple", 5});
      mm.insert({"banana", 3});
      mm.insert({"apple", 2});
  
      for (const auto& pair : mm) {
          std::cout << pair.first << ": " << pair.second << std::endl;
      }
      // 输出:
      // apple: 5
      // apple: 2
      // banana: 3
  }
  ```

### 5. `std::unordered_set` (C++11)
- **特点**: `std::unordered_set` 是一个无序的集合，其中的元素是唯一的。
- **底层实现**: 基于哈希表。
- **操作复杂度**: 插入、删除和查找操作的平均时间复杂度为 O(1)，最坏情况下为 O(n)。
- **示例**:
  ```cpp
  #include <iostream>
  #include <unordered_set>
  
  int main() {
      std::unordered_set<int> us;
      us.insert(3);
      us.insert(1);
      us.insert(4);
  
      for (int x : us) {
          std::cout << x << " ";  // 输出顺序不确定
      }
  }
  ```

### 6. `std::unordered_multiset` (C++11)
- **特点**: `std::unordered_multiset` 与 `std::unordered_set` 类似，但它允许存储重复的元素。
- **底层实现**: 基于哈希表。
- **操作复杂度**: 插入、删除和查找操作的平均时间复杂度为 O(1)，最坏情况下为 O(n)。
- **示例**:
  ```cpp
  #include <iostream>
  #include <unordered_set>
  
  int main() {
      std::unordered_multiset<int> ums;
      ums.insert(3);
      ums.insert(1);
      ums.insert(4);
      ums.insert(1);
  
      for (int x : ums) {
          std::cout << x << " ";  // 输出顺序不确定
      }
  }
  ```

### 7. `std::unordered_map` (C++11)
- **特点**: `std::unordered_map` 是一个无序的键值对容器，键是唯一的。
- **底层实现**: 基于哈希表。
- **操作复杂度**: 插入、删除和查找操作的平均时间复杂度为 O(1)，最坏情况下为 O(n)。
- **示例**:
  ```cpp
  #include <iostream>
  #include <unordered_map>
  
  int main() {
      std::unordered_map<std::string, int> um;
      um["apple"] = 5;
      um["banana"] = 3;
      um["orange"] = 8;
  
      for (const auto& pair : um) {
          std::cout << pair.first << ": " << pair.second << std::endl;
      }
      // 输出顺序不确定
  }
  ```

### 8. `std::unordered_multimap` (C++11)
- **特点**: `std::unordered_multimap` 与 `std::unordered_map` 类似，但它允许键重复。
- **底层实现**: 基于哈希表。
- **操作复杂度**: 插入、删除和查找操作的平均时间复杂度为 O(1)，最坏情况下为 O(n)。
- **示例**:
  ```cpp
  #include <iostream>
  #include <unordered_map>
  
  int main() {
      std::unordered_multimap<std::string, int> umm;
      umm.insert({"apple", 5});
      umm.insert({"banana", 3});
      umm.insert({"apple", 2});
  
      for (const auto& pair : umm) {
          std::cout << pair.first << ": " << pair.second << std::endl;
      }
      // 输出顺序不确定
  }
  ```

### 总结
- **有序关联容器** (`std::set`, `std::multiset`, `std::map`, `std::multimap`) 基于红黑树实现，元素按键排序，操作复杂度为 O(log n)。
- **无序关联容器** (`std::unordered_set`, `std::unordered_multiset`, `std::unordered_map`, `std::unordered_multimap`) 基于哈希表实现，元素无序，操作复杂度为 O(1) 平均情况下。

选择哪种关联容器取决于具体的应用场景和性能需求。如果需要有序性，可以选择有序关联容器；如果需要更高的性能并且不关心顺序，可以选择无序关联容器。

# Iterators

基本操作

```cpp
    std::vector<int> vec;
    std::vector<int>::iterator it = vec.begin(); // 创建迭代器
    std::cout << *it << std::endl; // 通过解引用获得迭代器原本的值
    ++ it; // 移动迭代器
    if(it == vec.end()) return; // 比较迭代器
```

## Iterators Type

1. Input
2. Output
3. Forward
4. Bidirectional
5. Random access

对于每种 Container 它的 Iterator 的类型是固定的。

对于所有的迭代器都可以

1. 从已经存在的迭代器创建
2. 用`++`向后移动一位
3. 使用`==`或`!=`进行比较

对于 Input 迭代器，它是只能读取值，也就是说他只能在表达式的右侧，并且它只能单次遍历。

对于 Output 迭代器，它是只能写入值，也就是说他只能出现在的表达式的左侧，并且它只能单次遍历。

对于 Forward 迭代器，它可以实现 Input 迭代器和 Output 迭代器的所有功能，并且可以实现多次遍历。

> **多次遍历**指的是能够对同一个序列（如容器或数据集合）进行多次从头到尾的访问。具体来说，如果一个迭代器支持多次遍历，意味着你可以通过它多次访问序列中的元素，而不需要重新创建或重置迭代器。

对于 Bidirectional 迭代器可以实现 Forward 迭代器的所有功能，并且有`--`操作

对于 Random access 迭代器可以 Bidirectional 的所有功能，并且实现任何值的加减，比如`iter = iter + 3`这样。

对于迭代器，如果用继承来看就是

- Random access
    - Bidirectional
        - Forward
            - Input
            - Output

并且，对于任何一种迭代器，我们都可以直接使用任何一种比他更高级的迭代器。

如何理解指针和迭代器的区别？

> 可以把指针理解为一种 class，而迭代器理解为一种 interface。这样指针就是 Random access Iterator 的一种实现。

# Templates

## Template Function

我们要实现一个函数`minmax`传入两个值，并返回最小值和最大值。

这个还是很好实现的

```cpp
#include <iostream>

using std::cout;

std::pair<int, int> minmax(int a, int b) {
    if (a < b) return {a, b};
    return {b, a};
}

int main() {
    auto [min1, max1] = minmax(2, -3);
    cout << min1 << " " << max1 << "\n";
    return 0;
}
```

然后我们考虑其他类型是否可以执行？

```cpp
#include <iostream>

using std::cout;

std::pair<int, int> minmax(int a, int b) {
    if (a < b) return {a, b};
    return {b, a};
}


int main() {
    auto [min1, max1] = minmax(2, -3);
    cout << min1 << " " << max1 << "\n";

    auto [min2, max2] = minmax(2.5, 3.1);
    cout << min2 << " " << max2 << "\n";

    auto [min3, max3] = minmax('a', 'l');
    cout << min3 << " " << max3 << "\n";

    return 0;
}
```

这依旧是可以执行的，因为浮点型和字符型都可以隐式类型转换位整型，但是结果是不对。

> 这样之所以可以编译，还是因为 "Allow the programmer full control, responsibility, and choice if they want it." 所以编译器认为程序员知道自己在做什么。

但是如果继续新增

```cpp
#include <iostream>

using std::cout;

std::pair<int, int> minmax(int a, int b) {
    if (a < b) return {a, b};
    return {b, a};
}


int main() {
    auto [min1, max1] = minmax(2, -3);
    cout << min1 << " " << max1 << "\n";

    auto [min2, max2] = minmax(2.5, 3.1);
    cout << min2 << " " << max2 << "\n";

    auto [min3, max3] = minmax('a', 'l');
    cout << min3 << " " << max3 << "\n";

    auto [min4, max4] = minmax("Alice", "Bob");
    cout << min4 << " " << max4 << "\n";

    return 0;
}
```

此时就无法进行编译了，因为字符串不能隐式类型转换为整型。为了解决这个问题我们可以用到 C 语言中的函数重载解决。

```cpp
#include <iostream>

using std::cout;

std::pair<int, int> minmax(int a, int b) {
    if (a < b) return {a, b};
    return {b, a};
}

std::pair<double, double> minmax(double a, double b) {
    if (a < b) return {a, b};
    return {b, a};
}

std::pair<char, char> minmax(char a, char b) {
    if (a < b) return {a, b};
    return {b, a};
}

std::pair<std::string, std::string> minmax(std::string a, std::string b) {
    if (a < b) return {a, b};
    return {b, a};
}

int main() {
    auto [min1, max1] = minmax(2, -3);
    cout << min1 << " " << max1 << "\n";

    auto [min2, max2] = minmax(2.5, 3.1);
    cout << min2 << " " << max2 << "\n";

    auto [min3, max3] = minmax('a', 'l');
    cout << min3 << " " << max3 << "\n";

    auto [min4, max4] = minmax("Alice", "Bob");
    cout << min4 << " " << max4 << "\n";

    return 0;
}
```

这样做既消除了编译的警告也可以正确的返回结果，但是我们注意到其实四个函数是非常冗余的。这里我们就可以引出**模板类型**这一概念。

```cpp
#include <iostream>

using std::cout;

template<typename T>
std::pair<T, T> minmax(T a, T b) {
    if (a < b) return {a, b};
    return {b, a};
}

int main() {
    auto [min1, max1] = minmax(2, -3);
    cout << min1 << " " << max1 << "\n";

    auto [min2, max2] = minmax(2.5, 3.1);
    cout << min2 << " " << max2 << "\n";

    auto [min3, max3] = minmax('a', 'l');
    cout << min3 << " " << max3 << "\n";

    auto [min4, max4] = minmax("Alice", "Bob");
    cout << min4 << " " << max4 << "\n";

    return 0;
}
```

这样做，也可以消除警告，并且大多数时候也可以正常的运行，唯独第四个的计算可能出现问题，我这里的结果是

```
-3 2
2.5 3.1
a l
Bob Alice
```

这是为什么？因为模板在识别`"Alice"`时，把他识别为`const char *`类型了。此次就会比较两个字符串常量的地址，而不是字典序？

对于这个问题我们可以在调用类型时指明参数的类型来实现。

```cpp
#include <iostream>
#include <array>

using std::cout;

template<typename T>
std::pair<T, T> minmax(T a, T b) {
    if (a < b) return {a, b};
    return {b, a};
}

int main() {
    auto [min1, max1] = minmax(2, -3);
    cout << min1 << " " << max1 << "\n";

    auto [min2, max2] = minmax(2.5, 3.1);
    cout << min2 << " " << max2 << "\n";

    auto [min3, max3] = minmax('a', 'l');
    cout << min3 << " " << max3 << "\n";

    auto [min4, max4] = minmax<std::string>("Alice", "Bob");
    cout << min4 << " " << max4 << "\n";

    auto [min5, max5] = minmax<std::array<int, 2>>({2, 4}, {1, 3});
    cout << min5[0] << " " << min5[1] << " , " << max5[0] << " " << max5[1] << "\n";
    return 0;
}
```

可以注意到，当你指明类型后，编译器就知道了如何进行类型转换，因此你甚至可以传入一些比较奇怪的东西。

关于模板函数，我们在再看下一个例子，这个例子是统计一个集合中某种元素出现的次数。

```cpp
#include <bits/stdc++.h>

using std::cout;

template<typename T>
int count(std::vector<T> set, T target) {
    int cnt = 0;
    for (size_t i = 0; i < set.size(); i += 1)
        if (set[i] == target) cnt++;
    return cnt;
}

int main() {
    cout << count({1, 1, 4, 5, 1, 4}, 1);
    return 0;
}
```

对于这个函数，我们默认了`arr`必须是一个`std::vector`才行，我们可以做一个优化。

```cpp
template<typename containerType, typename dataType>
int count(const containerType &set, dataType target) {
    int cnt = 0;
    for (size_t i = 0; i < arr.size(); i += 1)
        if (set[i] == target) cnt++;
    return cnt;
}
```

这样的话，理论上我们可以接受任何一种容器，但是并不是所有的容器都可以用`[]`进行下标访问。因此我们可以用迭代器实现。

```cpp
template<typename ContainerType, typename DataType>
int count(const ContainerType &set, DataType target) {
    int cnt = 0;
    for(auto iter = set.begin; iter != set.end; iter += 1)
        if(*iter == target) cnt ++;
    return cnt;
}
```

但是如果我们不需要整个集合的情况，我们改怎么办呢？可以不传入容器，而是传入两个类型的迭代器。

```cpp
template<typename InputIterator, typename DataType>
int count(const InputIterator &begin, const InputIterator end , DataType target) {
    int cnt = 0;
    for(auto iter = begin; iter != end; iter += 1)
        if(*iter == target) cnt ++;
    return cnt;
}
```

但是这个又对迭代器提出了要求，必须是Random access类型才行。因此我们可以在做使用`++`

```cpp
#include <iostream>
#include <set>
#include <vector>
#include <list>

using std::cout;

template<typename InputIterator, typename DataType>
int count(const InputIterator &begin, const InputIterator end, DataType target) {
    int cnt = 0;
    for (auto iter = begin; iter != end; iter++)
        if (*iter == target) cnt++;
    return cnt;
}

int main() {
    std::set<int> a{1, 1, 4, 5, 1, 4};
    std::vector<int> b{1, 1, 4, 5, 1, 4};
    std::list<int> c{1, 1, 4, 5, 1, 4};
    cout << count(a.begin(), a.end(), 1) << "\n";
    cout << count(b.begin(), b.end(), 1) << "\n";
    cout << count(c.begin(), c.end(), 1) << "\n";
    return 0;
}
```

这也是采用模板的一个重要作用就是可以实现**泛型编程**，对于最终的版本我们对于模板参数要求已经降低到了只需要保证`DataType`可以用`==`进行比较即可。

我们再来观察这个函数，这个函数目前实现的功能是查询集合中等于`target`的元素个数，我能否实现一些其他的功能？当然可以，模板参数不仅可以传入变量，也可以把函数传入进去。

```cpp
#include <iostream>
#include <set>
#include <vector>
#include <list>

using std::cout;

template<typename InputIterator, typename Predicate>
int count(const InputIterator &begin, const InputIterator end, Predicate pred) {
    int cnt = 0;
    for (auto iter = begin; iter != end; iter++)
        if (pred(*iter)) cnt++;
    return cnt;
}

bool lessThan3(int val) {
    return val < 3;
}

bool lessThan4(int val) {
    return val < 4;
}

bool lessThan5(int val) {
    return val < 5;
}

int main() {
    std::vector<int> a{1, 1, 4, 5, 1, 4, 1, 9, 1, 9, 8, 1, 0};
    cout << count(a.begin(), a.end(), lessThan3) << "\n";
    cout << count(a.begin(), a.end(), lessThan4) << "\n";
    cout << count(a.begin(), a.end(), lessThan5) << "\n";
    return 0;
}
```

这样就可以在不修改`count`函数的情况下修改`count`的功能，但是我们看`lessThan3,lessThan4,lessThan5`三个函数其实是大同小异的，有没有什么方法增加其通用性？当然我们可以把`lessThan`函数设计为两个参数，但是这样的话对于`count`函数就失去了通用性。

对于这个问题我们当然可以这样实现

```cpp
int limit;
bool lessThanLimit(int val) {
    return val < limit;
}
```

把`limit`作为一个全局变量，然后在每次调用`count`前修改`limit`的值即可。但是这样又会牵扯到一个问题，`limit`只在`lessThanlimit`中使用，作为全局变量不合适。

对于这个问题，我们可以用**Lambda**解决。

```cpp
int main() {
    std::vector<int> a{1, 1, 4, 5, 1, 4, 1, 9, 1, 9, 8, 1, 0};
    int limit = 0;
    auto lessThanLimit = [&limit](int val) {
        return val < limit;
    };
    limit = 3;
    cout << count(a.begin(), a.end(), lessThanLimit) << "\n";
    limit = 4;
    cout << count(a.begin(), a.end(), lessThanLimit) << "\n";
    limit = 5;
    cout << count(a.begin(), a.end(), lessThanLimit) << "\n";
    return 0;
}
```

当然了，其实这个问题我们可以用**非类型模板参数**解决。

```cpp
template<int limit>
bool lessThanLimit(int val) {
    return val < limit;
}

int main() {
    std::vector<int> a{1, 1, 4, 5, 1, 4, 1, 9, 1, 9, 8, 1, 0};
    cout << count(a.begin(), a.end(), lessThanLimit<3>) << "\n";
    cout << count(a.begin(), a.end(), lessThanLimit<4>) << "\n";
    cout << count(a.begin(), a.end(), lessThanLimit<5>) << "\n";
    return 0;
}
```

要注意的是，非类型模板参数只能接受常量，也就是必须在编译时就已知值才行。 

# OOP

## Const函数

- **不会修改对象状态**：`const` 成员函数承诺不会修改对象的非静态成员变量。如果函数尝试修改非静态成员变量，编译器会报错。
- **可以被 const 对象调用**：只有 `const` 成员函数可以被 `const` 对象调用。非 `const` 对象也可以调用 `const` 函数，但 `const` 对象不能调用非 `const` 函数。
- **不会调用非 const 成员函数**：`const` 函数内部不可以调用非 `const` 成员函数，除非非 `const` 函数指定了 `mutable` 成员变量。

关于`const`函数有两种

```cpp
void f(const int &x) {
    cout << x << "\n";
}
```

这里的 `const` 表示参数 `x` 是一个 `const` 引用，即函数 `f` 不能修改 `x` 的值。

```cpp
class myClass {
public:
    int x;

    void f() const {
        cout << x << "\n";
    }
};
```

这个`const` 限定符表示该成员函数不会修改对象的状态（即不会修改类的非静态成员变量）。如果函数尝试修改成员变量（如 `x`），编译器会报错。

## Const Pointer 

```cpp
int x;
const int y = 1;
// constant pointer to a non-constant int
int *const p1 = &x;
int *const p2 = &y;
// non-constant pointer to a constant int
const int *p2 = &y;
int const *p3 = &x;
// constant pointer to a constant int
const int *const p4 = &x;
int const *const p5 = &y;
```

- **数据是否可变**：`p1` 指向的数据可以被修改，而其他指针（`p2`、`p3`、`p4`、`p5`）指向的数据不能被修改。
- **指针是否可变**：
    - `p1` 是常量指针，不能重新指向。
    - `p2` 和 `p3` 是非常量指针，可以重新指向。
    - `p4` 和 `p5` 是常量指针，不能重新指向。
- **语法上的差异**：
    - `const int *p` 和 `int const *p` 是等价的。
    - `int *const p` 表示指针本身是常量。

值得注意的是`int *const p6 = &y;`是无法编译的，因为`y`是`const int` 但是`p6`只能指向非常量变量。

分析以下代码中`const`的作用

```cpp
const int * const myClassmethod(const int * const & pararm) const;
```

- 参数部分`const int * const &pararm` 

    - `const int *`表示指针指向了一个`const int`
    - `const`表示指针是`const`类型，不能被修改

- 返回值部分`const int * const`

    - `const int *` 返回值指针指向`const int`
    - `const`表示指针是`const`类型

- 成员函数`const`

    - 成员函数声明末尾的 `const` 表示该函数不会修改类的任何非静态成员变量

    参数中的`&`，表示的是参数是传引用。

## Const Iterators

```cpp
vector<int> a{1,23};
const vector<int>::iterator iter1 = a.begin();
iter1 ++;// doesnt compile
*iter1 = 4;// compile

vector<int>::const_iterator iter2 = a.begin();
iter2 ++;// compile
*iter2 = 4;// doesnt compile
```

# Operators

```cpp
vector<string> a{"Hello", "World"};
cout << a[0];
a[1] += "1";
```

我看先来看这个简单的例子，这个例子中用到了运算符，运算符实际上可以理解为成员函数，因此这里的写法实际上是简写。

```cpp
vector<string> a{"Hello", "World"};
cout.operator<<(a.operator[](0));
a.operator[](1) += "1";
```

上面说到了，运算符实际上就是函数，因此也可以这样写

```cpp
vector<string> a{"Hello", "World"};
operator<<(cout, operator[](a, 1));
operator+=(operator[](a, 1), "!");
```

我们再来看一个下面的例子

```cpp
#include <bits/stdc++.h>

using std::cout;
using std::vector;
using std::string;

using VecStr = vector<string>;

VecStr &operator+=(VecStr &vs, const string &element) {
    vs.push_back(element);
    return vs;
}

int main() {
    VecStr s;
    (((s += "Hello") += " ") += "World") += "!";
    for (auto i: s)cout << i;
    cout << "\n";
    return 0;
}
```

# Special Member Function

这里学习了两个特殊的函数 copy constructor 和  copy assignment。

```cpp
class String {
private:
    char* data;
public:
    // 拷贝构造函数（初始化新对象）
    String(const String& other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }

    // 拷贝赋值操作符（修改已存在对象）
    String& operator=(const String& other) {
        if (this != &other) { // 处理自赋值
            delete[] data;    // 释放旧资源
            data = new char[strlen(other.data) + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    ~String() { delete[] data; } // 析构函数
};
```

- **拷贝构造函数**：创建新对象时初始化其状态。
- **拷贝赋值操作符**：修改已存在对象的状态，需处理自赋值和资源释放。

# Move

*l-value和r-value只是两个简化的模型*

- **l-value** 是一个具有名称（身份）的表达式。
    - 可以使用取地址运算符（`&var`）找到其地址。
    - 可以出现在赋值语句左边或右边的表达式。
- **r-value** 是一个没有名称（身份）的表达式。
    - 临时值。
    - 不能使用取地址运算符（`&var`）找到其地址。
    - 只能出现在赋值语句右边的表达式。

下面再看一些简单的例子

```cpp
int val = 2; // val is l-value, 2 is r-value
int *ptr = 0x2248837;// *ptr is l-value, 0x2248837 is r-value

auto v3 = v1 + v2; // v3, v1, v2 is l-value, but v1 + v2 is r-value
size_t size = v3.size(); // size is l-value, v3.size() is r-value
v1[1] = 4 * i; // v1[1] is l-value, 4 * i is r-value
ptr = &val; // &val is r-value
v1[2] = *ptr; // *ptr is l-value
```

为什么`v3.size()`是r-value？因为`size()`的返回值是`size_t`不是`size_t&`。同理`v1[1]`是l-value是因为`operator[]`的返回值是`int&`。

然后再看两个例子

```cpp
auto& v2 = v1; // v2 is l-value reference
auto&& v3 = v1 + v2; // v3 is r-value reference
```

`v2`是左值引用，因为`v1`是左值，所以访问`v2`实际上就是访问`v1`

`v3`是右值引用，`v1 + v2`是右值，因此访问`v3`是访问计算`v1 + v2`产生的临时变量，这样做可以延长临时变量的生存周期，减小内存消耗。

正常情况左值引用是不能引用到右值的。但是常量左值引用除外。

```cpp
const auto& v4 = v1 + v2;
```

因为左值引用对于这如果修改了`v1`或`v2`的值，`v4`的值也会被修改。但是因为是常量所以`v4`的值是不能被改变的。

`r-value`只是一个临时值，用完后就会被立即销毁，我们是否可以通过利用`r-value` 来提高效率？

因此对于刚才的`String`我可以写出这样两个函数，如果构造函数的值或赋值的值是右值我就可以直接移动指针。

```cpp
String::String(String &&other) : data(other.data) { // Move Constuctor
    other.data = nullptr;
}

String &String::operator=(String &&other) { // Move Assignment
    if (this != &other) {
        delete[] data;
        ata = other.data;
        other.data = nullptr;
    }
    return *this;
}
```

这样写会减少很多内存的浪费。当然了这里用到的是指针可以直接赋值。但如果是类，我们可以使用`std::move`函数来实现。

```cpp
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
```

因此我也可以写出一个不增加额外内存消耗的`swap`函数

```cpp
template<typename T>
void swap(T &a, T &b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}
```

# Interface

```cpp
class Drink {
public:
    virtual void make() = 0;
};

class Tea : public Drink {
public:
    void make() override {
        return;
    }
};
```

由于C++中没有接口这个概念，因此如果要实现类似Java的接口可以采用这种方法实现。

首先定义了一个基类`Drink`，这个类包含了一个**纯虚函数**`make()`，因此`Dirnk`不能直接实例化，并且所有派生类都必须实现`make()`。

什么是纯虚函数？	

- **纯虚函数**是基类中声明但**不提供具体实现**的虚函数。
- **语法**：在虚函数声明末尾添加 `= 0`。
- **抽象类**：包含纯虚函数的类称为**抽象类**，不能直接实例化。
- **派生类责任**：派生类必须实现所有基类的纯虚函数，否则自身也会成为抽象类。
- **可选的基类实现**：纯虚函数可以有默认实现（但派生类仍需显式重写）。

与普通虚函数的区别

| **特性** | **纯虚函数**               | **普通虚函数**                 |
| :------- | :------------------------- | :----------------------------- |
| 实现要求 | 基类无实现，派生类必须重写 | 基类有默认实现，派生类可选重写 |
| 类性质   | 使类成为抽象类             | 普通类（可直接实例化）         |
| 语法     | `virtual void func() = 0;` | `virtual void func();`         |
| 用途     | 定义强制接口规范           | 提供可扩展的默认行为           |

`override` 的作用

- **明确意图** ：它明确告知编译器，当前函数的目标是覆盖基类的虚函数。如果基类中没有对应的虚函数（例如拼写错误或参数列表不匹配），编译器会报错。
- **防止拼写错误** ：避免因为函数名称拼写错误或参数列表不匹配而未覆盖基类函数的问题。例如：
    - 如果基类的虚函数是 `virtual void print();`，而派生类中写成 `void prin() override;`，编译器会报错，因为基类中没有 `prin()` 虚函数。

`class Tea : public Drink` 中的 `public` 是继承访问修饰符，用于指定基类 `Drink` 中成员在派生类 `Tea` 中的可见性。

- **`public` 继承**：
    - 基类 `Drink` 中的 **`public` 成员** 在派生类 `Tea` 中仍然是 **`public`**。
    - 基类 `Drink` 中的 **`protected` 成员** 在派生类 `Tea` 中仍然是 **`protected`**。

- **`protected` 继承**：
    - 基类 `Drink` 中的 **`public` 成员** 在派生类 `Tea` 中变为 **`protected`**。
    - 基类 `Drink` 中的 **`protected` 成员** 在派生类 `Tea` 中仍然是 **`protected`**。
    - 外部无法通过 `Tea` 的对象或指针访问 `make()` 方法。
- **`private` 继承**：
    - 基类 `Drink` 中的 **`public` 和 `protected` 成员** 在派生类 `Tea` 中变为 **`private`**。
    - 外部无法通过 `Tea` 的对象或指针访问 `make()` 方法。

为什么要有虚函数，C++是可以对非虚函数进行重写的。

```cpp
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
```

`ptr2`指向`tea`，但是这样做只能调用基类的`make()`函数。

| **场景**     | **非虚函数**                     | **虚函数**                                  |
| :----------- | :------------------------------- | :------------------------------------------ |
| **调用方式** | 静态绑定（编译时决定）           | 动态绑定（运行时决定）                      |
| **多态支持** | ❌ 无法通过基类指针调用派生类实现 | ✅ 支持基类接口调用不同派生类的实现          |
| **性能开销** | 无额外开销                       | 虚函数表查找的微小开销                      |
| **设计意图** | 提供固定实现，不希望被派生类修改 | 定义可扩展的接口，强制/允许派生类自定义行为 |

下面这个例子综合了目前学过的所有有关继承的知识。

```cpp
#include <iostream>

using std::cin;
using std::cout;

class Drink {
public:
    Drink() = default; // 显式声明默认构造函数

    Drink(const Drink &drink) = delete; // 禁用拷贝构造函数

    Drink(std::string flavor) : flavor(flavor) {}

    virtual void make() = 0; // 纯虚函数

    virtual void foo() { // 虚函数
        cout << "Drink " << flavor << "\n";
    }

    ~Drink() = default; // 显式声明默认析构函数

private:
    std::string flavor;
};

class Tea : public Drink {
public:
    Tea() = default;

    Tea(std::string flavor) : Drink(flavor) {}

    virtual void make() override { // 对父类的make函数重写，并且允许之类继续重写但不强制要求
        cout << "made from Tea class\n";
    }

    void foo() override {
        Drink::foo(); // 显式调用父类版本
        cout << "Tea\n";
    }

    ~Tea() = default;
};

int main() {
    Tea t("red");
    t.foo();
    t.make();
    t.Drink::foo(); // 直接访问父类的foo函数
}
```

# Template Class

基本语法

```cpp
template <typename T> // 或者 template <class T>
class Box {
    // 类成员和方法的定义，可以使用 T 作为类型占位符
private:
    T content;
public:
    Box(T value) : content(value) {}
    T getContent() { return content; }
};
```

模板实例化

```cpp
Box<int> intBox(42);        // T 被替换为 int
Box<std::string> strBox("Hello"); // T 被替换为 string
```

多类型参数

```cpp
template <typename T, typename U>
class Pair {
public:
    T first;
    U second;
    Pair(T a, U b) : first(a), second(b) {}
};

Pair<int, double> p1(10, 3.14);
Pair<std::string, bool> p2("key", true);
```

非类型模板参数

```cpp
template <typename T, int N>
class Array {
private:
    T data[N];
public:
    T& operator[](int index) { return data[index]; }
};

Array<double, 5> arr; // 创建一个大小为 5 的 double 数组
```

模板特化

```cpp
// 通用模板
template <typename T>
class Printer {
public:
    void print(T value) { std::cout << value << std::endl; }
};

// 特化为 bool 类型
template <>
class Printer<bool> {
public:
    void print(bool value) { 
        std::cout << (value ? "true" : "false") << std::endl; 
    }
};
```

## typename 和 class 的区别

在C++17 之前的版本：有当声明**模板模板参数**时**必须使用**`class`

```cpp
// 定义一个接受模板模板参数的类
template <
    template <class T> class Container  // 正确：使用 class（兼容所有标准）
>
class MyClass {
    Container<int> data;  // 使用模板模板参数实例化
};

// C++17 起也可以用 typename（但部分编译器可能不完全支持）
template <
    template <typename T> typename Container  // C++17 允许
>
class MyClassV2 { /* ... */ };
```

## C++20 concept 

在C++20 中引入了`concept`概念以在编译期检查模板实参是否满足指定的约束。

先看这个例子

```cpp
template<typename T>
T foo(T a) {
    return ++a;
}
```

这个例子其实对`a`是限制的，即必须要求`a`必须有`operator++`。但这个实际上是隐式要求，只有当编译的时候才能体现出来。为了方便介绍，我们先强制要求为整型。

我们可以先用`concept` 定义约束

```cpp
template<typename T>
concept integral = std::is_integral<T>::value;
```

`std::is_integral`是一个**类型特征**，可以判断一个值是否是整型。如果要知道结果就要调用`value`，其返回值是一个`bool`类型。

| 特征名称                   | 功能描述                                       | 示例                                                |
| -------------------------- | ---------------------------------------------- | --------------------------------------------------- |
| `std::is_integral`         | 检查类型是否为整数类型（如 `int`, `long` 等）  | `std::is_integral<int>::value == true`              |
| `std::is_floating_point`   | 检查类型是否为浮点类型（如 `float`, `double`） | `std::is_floating_point<float>::value == true`      |
| `std::is_arithmetic`       | 检查类型是否为算术类型（整数或浮点）           | `std::is_arithmetic<int>::value == true`            |
| `std::is_same`             | 检查两个类型是否相同                           | `std::is_same<int, int>::value == true`             |
| `std::is_const`            | 检查类型是否为常量类型                         | `std::is_const<const int>::value == true`           |
| `std::is_volatile`         | 检查类型是否为易失性类型                       | `std::is_volatile<volatile int>::value == true`     |
| `std::is_pointer`          | 检查类型是否为指针类型                         | `std::is_pointer<int*>::value == true`              |
| `std::is_reference`        | 检查类型是否为引用类型                         | `std::is_reference<int&>::value == true`            |
| `std::is_lvalue_reference` | 检查类型是否为左值引用类型                     | `std::is_lvalue_reference<int&>::value == true`     |
| `std::is_rvalue_reference` | 检查类型是否为右值引用类型                     | `std::is_rvalue_reference<int&&>::value == true`    |
| `std::remove_const`        | 去除类型的常量限定符                           | `typename std::remove_const<const int>::type`       |
| `std::remove_volatile`     | 去除类型的易失性限定符                         | `typename std::remove_volatile<volatile int>::type` |
| `std::remove_reference`    | 去除类型的引用限定符                           | `typename std::remove_reference<int&>::type`        |
| `std::remove_cv`           | 去除类型的常量和易失性限定符                   | `typename std::remove_cv<const volatile int>::type` |

- **`_v` 后缀**：C++17 引入的模板变量，用于直接获取类型特征的布尔值。
    例如：`std::is_integral_v<T>` 等价于 `std::is_integral<T>::value`。
- **`_t` 后缀**：用于获取类型特征的别名。
    例如：`std::remove_const_t<const int>` 等价于 `int`。

在定义好约束后，我们可以用以下方法要求`T`满足这个约束，这四种写法是等价的。

```cpp
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
```

当然对于约束，我们也可以进行嵌套

```cpp
template<typename T>
concept signed_integral = integral<T> and std::is_signed<T>::value;

template<typename T>
concept unsigned_integral = integral<T> && std::is_unsigned_v<T>;
```

当然了对于上述的要求我们也可自己实现一些要求，比如

```cpp
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
```

# RAII

## 内存泄漏的主要原因

1. **手动管理失误**
    - 使用 `new` 分配内存后，未调用 `delete` 释放。
    - 数组未用 `delete[]` 释放，或错误使用 `free()` 释放 `new` 分配的内存。
2. **异常或中断**
    - 在 `delete` 前发生异常、`return` 或分支跳转，导致释放代码未执行。
3. **循环引用（智能指针）**
    - `shared_ptr` 互相引用时，引用计数无法归零，需结合 `weak_ptr` 解决。
4. **未正确释放资源**
    - 基类析构函数未声明为 `virtual`，导致派生类资源未释放。



## 异常（Exception）

异常是 C++ 中用于处理程序运行时错误的一种机制，允许程序在检测到错误时跳出当前执行流程，并通过特定代码块（`catch`）处理错误。其核心是 **分离正常逻辑与错误处理**，避免代码被大量错误检查污染。

1. **`throw`**
    - **抛出异常**：当检测到错误时，用 `throw` 抛出一个异常对象（可以是任意类型，但建议用标准异常类或自定义类）。
    - 示例：`throw std::runtime_error("File not found");`

1. **`try`**

    - **监控代码块**：将可能抛出异常的代码包裹在 `try` 块中。

    - 示例：

        ```cpp
        try {
            // 可能抛出异常的代码
            openFile("data.txt");
        }
        ```

2. **`catch`**

    - **捕获并处理异常**：根据异常类型匹配对应的 `catch` 块，执行错误处理逻辑。

    - 示例:

        ```cpp
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        ```

完整例子

```cpp
#include <iostream>
#include <stdexcept>

double divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("除数不能为0"); // 抛出异常
    }
    return static_cast<double>(a) / b;
}

int main() {
    try {
        double result = divide(10, 0); // 可能抛出异常的调用
        std::cout << "结果: " << result << std::endl;
    } catch (const std::invalid_argument &e) { // 捕获特定异常
        std::cerr << "捕获异常: " << e.what() << std::endl;
    } catch (...) { // 捕获所有其他异常
        std::cerr << "未知错误" << std::endl;
    }
    return 0;
}

```

- 异常类型匹配时`catch`按照顺序匹配异常类型，并支持继承关系，如捕获基类异常`std::exception`可处理所有派生类异常。

- 抛出异常后，程序会逐层退出函数调用栈，直到找到匹配的 `catch` 块，同时自动调用局部对象的析构函数（需确保析构函数不抛异常）。

- C++ 在`<stdexpect>`提供了标准异常类，可以用`what()`获取错误信息

    - `logic_error` 程序逻辑错误

    - `invalid_argument` 逻辑错误：无效参数

    - `domain_error` 逻辑错误：参数对应的结果值不存在

    - `length_error` 逻辑错误：试图创建一个超出该类型最大长度的对象

    - `out_of_range` 逻辑错误：使用一个超出有效范围的值

    - `runtime_error` 运行时错误

    - `range_error` 运行时错误：生成的结果超出了有效值的范围

    - `overflow_error` 运行时错误：计算上溢

    - `underflow_error` 运行时错误：计算下溢

## 异常安全

函数可以具有四个级别的异常安全：

- **不抛异常保证（Nothrow exception guarantee）**
    - 绝对不会抛出异常：析构函数、交换操作、移动构造函数等。
- **强异常安全保证（Strong exception guarantee）**
    - 回滚到函数调用之前的状态。
- **基本异常安全保证（Basic exception guarantee）**
    - 异常后程序处于有效状态。
- **无异常保证（No exception guarantee）**
    - 资源泄漏、内存损坏、严重错误等。

## RAII

RAII 的核心思想是将资源的获取与对象的构造绑定在一起，而资源的释放则与对象的析构绑定在一起。当对象超出作用域时，析构函数会自动被调用，从而释放资源。这种机制可以确保即使在发生异常的情况下，资源也能被正确释放。

```cpp
void printFile() {
    ifstream input;
    input.open("hamlet.txt");
    string line;
    while (getline(input, line))
        std::cout << line << std::endl;
    input.close();
}
```

这样的一段代码就不符合RAII，因为获取资源和释放资源都是通过条用函数实现的，如果在中间出现了异常，则`input.close()`将无法正常执行。

```cpp
void printFile() {
    ifstream input("hamlet.txt");
    string line;
    while (getline(input, line))
        std::cout << line << std::endl;
}
```

其实只要这样修改就可以满足RAII了

## 智能指针

```cpp
void foo() {
    Node *n = new Node();
    // do somthing
    delete n;
}
```

我们看这个函数，实际上这里的指针是不符合RAII的。如何解决这个问题？我们可以用智能指针实现，智能指针实际上就是一个类，我们用构造函数获取资源，在用析构函数释放资源。

C++ 的标准库中已经准备了三种智能指针

```cpp
std::unique_ptr;
std::shared_ptr;
std::weak_ptr;
```

`std::unique_ptr` 不允许被复制，因为复制会导致一个资源不被唯一占用，还可能会导致资源被重复释放产生安全漏洞。

对于刚才的例子，用智能指针实现也很简单。

```cpp
void foo1() {
    std::unique_ptr<Node> n(new Node);
    // do somthing
    // free!
}
```

如果确实需要复制指针，或者说确实需要多个指针指向一个对象，我们可以用`std::shared_prt`

```cpp
{
    std::shared_ptr<int> p1(new int);
    // use p1
    {
        std::shared_ptr<int> p2(p1);
        std::shared_ptr<int> p3 = p2;
        // use p1, p2, p3
    }
    // use p1
}
// free
```

只有当指向一个对象所有的共享指针都离开了作用域时，这个对象才会被释放。

共享指针是如何实现的？对于同一个对象，共享指针维护了一个**引用计数器**，每拷贝一次计数器就会加一，每一个指针离开作用域计数器就会减一。当计数器为0时就会释放资源。

对于`std::week_ptr`他和`std::shared_ptr`比较类似，但是当他增加时，并不会使得引用计数器加一。

### 相同点

| 特性                 | 说明                                                      |
| :------------------- | :-------------------------------------------------------- |
| **自动释放内存**     | 离开作用域时自动释放资源，避免内存泄漏。                  |
| **支持自定义删除器** | 可通过模板参数指定删除器（如 `unique_ptr<T, Deleter>`）。 |
| **操作符重载**       | 支持 `operator*` 和 `operator->`，行为类似裸指针。        |
| **RAII原则**         | 资源生命周期与对象绑定，确保异常安全。                    |

###  不同点

| 特性             | `unique_ptr`                 | `shared_ptr`                          | `weak_ptr`                       |
| :--------------- | :--------------------------- | :------------------------------------ | :------------------------------- |
| **所有权**       | 独占资源                     | 共享资源                              | 不拥有资源                       |
| **拷贝语义**     | 不可拷贝，只能移动           | 可拷贝，引用计数递增                  | 可拷贝，但引用计数不变           |
| **引用计数**     | 无                           | 有（`use_count()`）                   | 无（但依赖 `shared_ptr` 的计数） |
| **性能开销**     | 无                           | 有（原子操作维护引用计数）            | 低（仅观察）                     |
| **循环引用问题** | 无                           | 可能导致循环引用（需配合 `weak_ptr`） | 用于解决循环引用                 |
| **典型使用场景** | 独占动态对象、工厂模式返回值 | 共享资源（如缓存、多线程数据）        | 观察者模式、打破循环引用         |

### 选择指南

- **优先使用 `unique_ptr`**：资源无需共享时，优先选择（性能最优）。
- **共享资源用 `shared_ptr`**：需要多个所有者时使用，但需注意循环引用风险。
- **观察资源用 `weak_ptr`**：配合 `shared_ptr` 使用，避免循环引用。

