#include <iostream>
using namespace std;

int main() {
    int a = 10;
    
    // 指针：可以指向别处，可以为空
    int* ptr = &a;
    cout << "ptr = " << ptr << ", *ptr = " << *ptr << endl;
    int b = 20;
    ptr = &b;          // 指针可以重新指向
    cout << "ptr 改指向 b: *ptr = " << *ptr << endl;
    
    // 引用：绑定后不能改变，不能为空
    int& ref = a;
    cout << "ref = " << ref << endl;
    // int& ref2;      // 编译错误：引用必须初始化
    // ref = b;        // 注意！这不是让ref指向b，而是把b的值赋给a
    cout << "ref 现在 = " << ref << " (注意 ref 还是 a 的别名)" << endl;
    
    // 引用的底层实现（用指针模拟）
    int* const hidden_ptr = &a;   // 引用本质上相当于一个 const 指针
    cout << "hidden_ptr = " << hidden_ptr << ", *hidden_ptr = " << *hidden_ptr << endl;
    
    return 0;
}