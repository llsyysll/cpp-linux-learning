#include <iostream>
using namespace std;

int main() {
    // const修饰普通变量
    const int a = 10;
    // a = 20;  // 编译错误
    cout << "a = " << a << endl;

    // const变量可以用非常量初始化
    int b = 30;
    const int c = b;
    b = 40;
    cout << "c = " << c << endl;  // c还是30
    
    return 0;
}