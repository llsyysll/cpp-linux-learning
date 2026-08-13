#include <iostream>
#include <cstdlib>
using namespace std;

class MyClass {
public:
    int a, b;
    MyClass(int x, int y) : a(x), b(y) {
        cout << "构造: a=" << a << ", b=" << b << endl;
    }
    ~MyClass() {
        cout << "析构" << endl;
    }
};

int main() {
    void* raw_mem = malloc(sizeof(MyClass));
    cout << "原始内存地址: " << raw_mem << endl;

    MyClass* obj = new (raw_mem) MyClass(10, 20);
    cout << "对象地址: " << obj << endl;

    obj->~MyClass();
    free(raw_mem);

    return 0;
}