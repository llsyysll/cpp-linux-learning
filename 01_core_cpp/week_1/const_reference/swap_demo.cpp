#include <iostream>
using namespace std;

// 方式1：传值（❌ 无效，因为操作的是副本）
void swap_by_value(int a, int b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// 方式2：传指针（✅ 有效，但语法繁琐）
void swap_by_pointer(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// 方式3：传引用（✅✅ 最优雅，推荐）
void swap_by_reference(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// 方式4：用 std::swap（C++标准库，最省事）
// #include <algorithm> 然后 std::swap(a, b)

int main() {
    int x = 10, y = 20;
    
    swap_by_value(x, y);
    cout << "传值交换后: x = " << x << ", y = " << y << " (没变)" << endl;
    
    swap_by_pointer(&x, &y);
    cout << "传指针交换后: x = " << x << ", y = " << y << " (成功了)" << endl;
    
    // 恢复原始值再测试
    x = 10; y = 20;
    swap_by_reference(x, y);
    cout << "传引用交换后: x = " << x << ", y = " << y << " (成功了，更简洁)" << endl;
    
    return 0;
}