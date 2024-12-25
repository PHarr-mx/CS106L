# include <bits/stdc++.h>

using namespace std;

int stringToInter(const string &s) {
    istringstream iss(s);
    int result;
    iss >> result;
    return result;
}

int main(){

    cout << stringToInter("123") / 2;
}