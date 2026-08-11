#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int b = 20;
    
    // const int*：指向的内容不可变，指针本身可变
    const int* p1 = &a;
    // *p1 = 30;   // 编译错误
    cout << "*p1 = " << *p1 << endl;
    p1 = &b;        // 指针可以改变指向
    cout << "*p1 = " << *p1 << endl;
    
    // int* const：指针本身不可变，指向的内容可变
    int* const p2 = &a;
    *p2 = 30;       // 可以修改指向的内容
    cout << "*p2 = " << *p2 << endl;
    // p2 = &b;     // 编译错误
    
    // const int* const：两者都不可变
    const int* const p3 = &a;
    // *p3 = 40;    // 编译错误
    // p3 = &b;     // 编译错误
    
    return 0;
}