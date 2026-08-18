#include <iostream>
#include <cstring>
using namespace std;


// ============================================
// 1. const修饰变量
// ============================================
void test_const_var() {
    cout << "\n=== 1. const修饰变量 ===" << endl;
    const int a = 10;
    // a = 20;  // 编译错误
    cout << "a = " << a << endl;

    int b = 30;
    const int c = b;
    b = 40;
    cout << "c = " << c << " (b改变后c不变，因为是拷贝)" << endl;
}

// ============================================
// 2. const修饰指针（三种情况）
// ============================================
void test_const_pointer() {
    cout << "\n=== 2. const修饰指针 ===" << endl;
    int a = 10, b = 20;

    // 情况1：const int* p → 指向内容不可变，指针本身可变
    const int* p1 = &a;
    // *p1 = 30;  // 编译错误
    cout << "*p1 = " << *p1 << " (指向a)";
    p1 = &b;
    cout << " → 改为指向b: *p1 = " << *p1 << " (指针本身可变)" << endl;

    // 情况2：int* const p → 指针本身不可变，指向内容可变
    int* const p2 = &a;
    *p2 = 30;
    cout << "*p2 = " << *p2 << " (修改了指向的内容)";
    // p2 = &b;  // 编译错误
    cout << " (p2本身不能改变指向)" << endl;

    // 情况3：const int* const p → 两者都不可变
    const int* const p3 = &a;
    // *p3 = 40;  // 编译错误
    // p3 = &b;   // 编译错误
    cout << "*p3 = " << *p3 << " (指针和内容都不可变)" << endl;
}

// ============================================
// 3. const修饰成员函数
// ============================================
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

    // const成员函数：不能修改成员变量
    int getValue() const {
        // value = 100;  // 编译错误
        return value;
    }

    const char* getName() const {
        // name = nullptr;  // 编译错误
        return name;
    }

    // 非const成员函数：可以修改成员变量
    void setValue(int v) { value = v; }
    char* getName() { return name; }
};

void test_const_member() {
    cout << "\n=== 3. const修饰成员函数 ===" << endl;
    
    const MyClass obj1(10, "Hello");
    cout << "const对象 → getValue() = " << obj1.getValue() << endl;
    cout << "const对象 → getName() = " << obj1.getName() << " (调用const版本)" << endl;

    MyClass obj2(20, "World");
    cout << "非const对象 → getValue() = " << obj2.getValue() << endl;
    cout << "非const对象 → getName() = " << obj2.getName() << " (调用非const版本)" << endl;
}

// ============================================
// 4. const修饰返回值
// ============================================
const int getConstValue() {
    return 42;
}

const string& getConstRef(const string& input) {
    return input;
}

void test_const_return() {
    cout << "\n=== 4. const修饰返回值 ===" << endl;
    
    int x = getConstValue();
    // getConstValue() = 100;  // 编译错误
    cout << "getConstValue() = " << x << " (只能读取，不能赋值)" << endl;

    string s = "Hello";
    const string& ref = getConstRef(s);
    // ref = "World";  // 编译错误
    cout << "getConstRef(s) = " << ref << " (返回const引用，不能修改)" << endl;
}

// ============================================
// main - 运行所有测试
// ============================================
int main() {
    cout << "========== const 用法综合测试 ==========";
    test_const_var();
    test_const_pointer();
    test_const_member();
    test_const_return();
    cout << "\n========== 所有测试完成 ==========\n";
    return 0;
}