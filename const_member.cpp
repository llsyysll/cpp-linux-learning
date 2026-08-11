#include <iostream>
#include <cstring>
using namespace std;

class MyClass {
private:
    int value;
    char* name;
    
public:
    MyClass(int v, const char* n) : value(v) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
    ~MyClass() { delete[] name; }
    
    // const成员函数
    int getValue() const { 
        // value = 100;  // 编译错误
        return value; 
    }
    
    const char* getName() const {
        // name = nullptr;  // 编译错误
        return name;
    }
    
    // 非const成员函数
    void setValue(int v) { value = v; }
    char* getName() { return name; }
};

int main() {
    // const对象只能调用const成员函数
    const MyClass obj1(10, "Hello");
    cout << "obj1.value = " << obj1.getValue() << endl;
    cout << "obj1.name = " << obj1.getName() << endl;
    
    // 非const对象优先调用非const版本
    MyClass obj2(20, "World");
    cout << "obj2.value = " << obj2.getValue() << endl;
    cout << "obj2.name = " << obj2.getName() << endl;
    
    return 0;
}