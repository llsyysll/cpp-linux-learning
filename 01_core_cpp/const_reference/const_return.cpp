#include <iostream>
using namespace std;

const int getConstValue() {
    return 42;
}

const string& getConstRef(const string& input) {
    return input;
}

int main() {
    int x = getConstValue();
    // getConstValue() = 100;  // 编译错误
    cout << "x = " << x << endl;
    
    string s = "Hello";
    const string& ref = getConstRef(s);
    cout << "ref = " << ref << endl;
    // ref = "World";  // 编译错误
    
    return 0;
}