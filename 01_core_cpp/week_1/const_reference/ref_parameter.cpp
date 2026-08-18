#include <iostream>
using namespace std;

// 传值：拷贝一份，原值不变
void passByValue(int x) {
    x = 100;
}

// 传指针：可以修改原值
void passByPointer(int* x) {
    *x = 200;
}

// 传引用：语法简洁，效率高，能修改原值
void passByReference(int& x) {
    x = 300;
}

// const引用：能读不能写，避免拷贝开销
void printByConstRef(const int& x) {
    cout << "const ref: x = " << x << endl;
    // x = 999;  // 编译错误：不能修改
}

int main() {
    int a = 10;
    
    passByValue(a);
    cout << "传值后 a = " << a << endl;    // 还是 10
    
    passByPointer(&a);
    cout << "传指针后 a = " << a << endl;  // 变成 200
    
    passByReference(a);
    cout << "传引用后 a = " << a << endl;  // 变成 300
    
    printByConstRef(a);                     // const 引用可接受 const 和非 const
    
    // 特别优势：const引用可以绑定到临时对象
    printByConstRef(42);                    // OK！const int& 可以绑定到右值
    // printByReference(42);               // 编译错误：普通引用不能绑定到右值
    
    return 0;
}