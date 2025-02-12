# Hash Map

[作业说明](https://web.stanford.edu/class/archive/cs/cs106l.1236/assignment2.html)

##  Milestone 1: Const-Correctness

首先打开`main.cpp`，找到`student_main()`函数，这个函数是用来测试里程碑 1 的。首先这个函数本身应该是没有 bug 的但是这个函数调用函数的参数应该是`const`类型。

首先需要修改这些函数的定义。我修改了
```cpp
//void print_difference(HashMap<string, int> &lecturer_record, string &lecturer1, string &lecturer2);
void print_difference(const HashMap<string, int> &lecturer_record, const string &lecturer1, const string &lecturer2);

//std::set<KeyType> find_keys(HashMap<KeyType, MappedTyped> &map);
template<typename KeyType, typename MappedTyped>
std::set<KeyType> find_keys(const HashMap<KeyType, MappedTyped> &map);
```
这样的话，函数就无法正常编译，因为有些`HashMap`有些成员函数也应该是`const`

所以要去`hashmap.cpp`和`hashmap.h`中重载一些函数。