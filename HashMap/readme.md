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

下一个需要重载的函数是

```cpp
iterator find(const K &key);
```

重载为

```cpp
const_iterator find(const K &key) const;
```

函数实现为

```cpp
template<typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::find(const K &key) const {
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H> *>(this)->find(key));
}
```



# Milestone 2: Special Member Functions and Move Semantics

去掉注释后，我发现原本可以正常编译的代码无法编译了。

这是因为我们没有一些成员函数定义为`const`，我把`size(),empty(),load_factor(),bucket_count(),contains()`的定义修改后就可以正常运行。

我们需要实现以下四个函数

## 拷贝构造函数

函数定义

```cpp
HashMap(const HashMap &other);
```

函数实现

`HashMap` 有三个变量，分别是`_size`内部元素个数，`_hash_function`哈希函数，`_buckets_array`桶数组。哈希函数不变很好理解。对于元素的值，我们不能直接拷贝桶数组，这样会直接指向原始的对象，并不会拷贝对象。因此我可以新建全部为`nullptr`的桶数组，并把个数设为零，然后逐个把元素插入到新的`HashMap`就好。

```cpp
template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(const HashMap<K, M, H> &other)
        :_size(0), _hash_function(other._hash_function),
         _buckets_array(other.bucket_count(), nullptr) {
    for (const_iterator iter = other.begin(); iter != other.end(); iter++) {
        this->insert(*iter);
    }
}

```

## 拷贝赋值函数

函数定义

```cpp
HashMap &operator=(const HashMap &other);
```

函数实现

首先我们要判断左值和右值是否是一个对象，如果不是一个对象才需要进行赋值。

如果要赋值，则首先应该把左值都清空。然后应当把左值的哈希函数赋值为右值的哈希函数。

要注意此时左值和右值的桶数组大小可能是不同的，我们要注意把桶的大小对应上。

最后把右值的值逐个插入即可。

```cpp
template<typename K, typename M, typename H>
HashMap<K, M, H> &HashMap<K, M, H>::operator=(const HashMap<K, M, H> &other) {
    if (this != &other) {
        this->clear();
        this->_hash_function = other._hash_function;
        this->_buckets_array.resize(other._buckets_array.size(), nullptr);
        for (const_iterator iter = other.begin(); iter != other.end(); iter++) {
            this->insert(*iter);
        }
    }
    return *this;
}
```

## 移动构造函数

函数定义

```cpp
HashMap(HashMap &&other);
```

因为全部都是对象，所以其实我们直接初始话列表加`std::move`就能解决

```cpp
template<typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(HashMap<K, M, H> &&other)
        :_size(std::move(other._size)),
         _hash_function(std::move(other._hash_function)),
         _buckets_array(std::move(other._buckets_array)) {}
```

## 移动赋值函数

函数定义

```cpp
HashMap &operator=(HashMap &&other);
```

函数实现

整体思路与移动构造函数很接近

```cpp
template<typename K, typename M, typename H>
HashMap<K, M, H> &HashMap<K, M, H>::operator=(HashMap<K, M, H> &&other) {
    if (this != &other) {
        this->clear();
        this->_size = std::move(other._size);
        this->_hash_function = std::move(other._hash_function);
        this->_buckets_array = std::move(other._buckets_array);
    }
    return *this;
}
```

