#include <bits/stdc++.h>

using namespace std;

class Node {

};


void foo() {
    Node *n = new Node();
    // do somthing
    delete n;
}

void foo1() {
    std::unique_ptr<Node> n(new Node);
    // do somthing
    // free!
}

void printFile() {
    ifstream input("hamlet.txt");
    string line;
    while (getline(input, line))
        std::cout << line << std::endl;
}

void foo2()
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

int main() {

    return 0;
}
