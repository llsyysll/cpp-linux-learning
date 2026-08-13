#include <iostream>
using namespace std;

// 错误❌：返回局部变量的引用（悬垂引用）
int& badReturn() {
    int local = 10;
    return local;  // 警告：local 在函数返回后销毁
}

// 正确✅：返回静态变量或全局变量的引用
int& goodReturn() {
    static int static_val = 10;
    return static_val;  // OK：静态变量生命周期贯穿整个程序
}

// 正确✅：返回传入的引用（链式调用）
int& chainReturn(int& x) {
    x += 1;
    return x;
}

// 思考题：返回 const 引用
const int& getConstRef() {
    static int val = 100;
    return val;
}

int main() {
    // 错误示例（千万别这么写）
    // int& ref = badReturn();
    // cout << ref << endl;  // 未定义行为！可能输出垃圾值
    
    // 正确示例1：静态变量
    int& r1 = goodReturn();
    cout << "goodReturn() = " << r1 << endl;
    r1 = 20;
    cout << "修改后 = " << goodReturn() << endl;
    
    // 正确示例2：链式调用
    int a = 0;
    chainReturn(chainReturn(a)) = 100;  // 第一次返回引用，第二次继续修改
    cout << "链式调用后 a = " << a << endl;  // 100
    
    // 正确示例3：const引用
    const int& r2 = getConstRef();
    cout << "const ref = " << r2 << endl;
    // r2 = 200;  // 编译错误：不能修改
    
    return 0;
}