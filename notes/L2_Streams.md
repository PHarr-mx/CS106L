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

