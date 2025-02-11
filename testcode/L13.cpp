#include <iostream>
#include <set>
#include <vector>
#include <list>

using std::cout;
using std::vector;


int main() {
    vector<int> a{1,23};
    const vector<int>::iterator iter1 = a.begin();
    iter1 ++;// doesnt compile
    *iter1 = 4;// compile
    vector<int>::const_iterator iter2 = a.begin();
    iter2 ++;// compile
    *iter2 = 4;// doesnt compile
    return 0;
}