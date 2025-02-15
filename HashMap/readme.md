# Hash Map

[作业说明](https://web.stanford.edu/class/archive/cs/cs106l.1236/assignment2.html)

## Milestone 1: Const-Correctness

首先打开`main.cpp`，找到`student_main()`函数，这个函数是用来测试里程碑 1 的。首先这个函数本身应该是没有 bug
的但是这个函数调用函数的参数应该是`const`类型。

首先需要修改这些函数的定义。我修改了

```cpp
//void print_difference(HashMap<string, int> &lecturer_record, string &lecturer1, string &lecturer2);
void print_difference(const HashMap<string, int> &lecturer_record, const string &lecturer1, const string &lecturer2);

//std::set<KeyType> find_keys(HashMap<KeyType, MappedTyped> &map);
template<typename KeyType, typename MappedTyped>
std::set<KeyType> find_keys(const HashMap<KeyType, MappedTyped> &map);
```

这样的话，函数就无法正常编译，因为有些`HashMap`有些成员函数也应该是`const`

所以要去`hashmap.cpp`和`hashmap.h`中重载一些函数。 我们逐个错误去去解决。

首先第一个错误是`map.end()`报错，这里应该是返回一个常量的。找到函数的声明是

```cpp
iterator end();
```

应当重载为

```cpp
const_iterator end() const;
```

为什么不能重载为`const_iterator end()`？因为C++不支持仅基于返回值类型重载。

考虑对于函数的重载，真的需要完整的实现？其实并不是，因为我们可以使用`static_cast` 和 `const_cast` 实现代码的复用。

先说`static_cast`函数，本质上是显示类型转换，有些类似C语言的强制类型转换。主要的差别在运算过从中进行类型检查，以避免隐式类型转换和其他不安全的类型转换。

```cpp
static_cast<int>(1.2);
(int)1.2;
```

这两句话在这里的作用是一样的。

再说`const_cast`的作用是解除对`const`关键字的限制。如果你需要在某些情况下修改一个 `const` 变量的值，但编译器不允许直接修改，可以使用
`const_cast`。换句话来说就是将 `const T*` 转换为 `T*`，或 `const T&` 转换为 `T&`。

好，接下来说一下我们重载的思路。我们首先调用`iterator end()`函数，并对返回值做类型转换。

我们通过`auto ptr = this`, 结合 CLion 的提示就可以知道`this` 的类型是`const HashMap<K, M, H> *`, 这也就意味着如果
`this -> end()` 调用的函数只能是`const_iterator end() const`，也就变成了递归调用。如何解决这个问题？我们可以
`const_cast<HashMap<K, M, H> *>(this)`去掉`const`关键字的限制，这样就可以正常调用`iterator end()`
所以最终这样就好了。
```cpp
template<typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::end() const {
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H> *>(this)->end());
}

```

下一个需要重载的函数就是
```cpp
M &at(const K &key);
```
重载为
```cpp
const M &at(const K &key) const;
```

函数实现就是

```cpp
template<typename K, typename M, typename H>
const M &HashMap<K, M, H>::at(const K &key) const {
    return static_cast<const M&>(const_cast<HashMap<K, M, H> *>(this)->at(key));
}
```

