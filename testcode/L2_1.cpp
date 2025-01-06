# include <bits/stdc++.h>

using namespace std;

void printStateBits(const istream & iss) {
    cout << "State bits: ";
    cout << (iss.good() ? "G" : "-");
    cout << (iss.fail() ? "F" : "-");
    cout << (iss.eof() ? "E" : "-");
    cout << (iss.bad() ? "B" : "-");
    cout << "\n";
}

int stringToInter(const string &s) {
    istringstream iss(s);
    printStateBits(iss);
    int result;
    iss >> result;
    printStateBits(iss);
    return result;
}

int main(){

    cout << stringToInter("100000000000000000000000000000000000000000000000") / 2;
}