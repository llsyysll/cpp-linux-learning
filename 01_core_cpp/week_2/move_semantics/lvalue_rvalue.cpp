// lvalue_rvalue.cpp
// 左值、右值、将亡值的辨析
// 编译：g++ -std=c++17 lvalue_rvalue.cpp -o lvalue_rvalue
// 运行：./lvalue_rvalue

#include <iostream>
#include <vector>
#include <string>

// ============================================
// 判断一个表达式是左值还是右值的关键
// ============================================
// 左值 (lvalue)  ：有持久地址的表达式，可以取地址
// 右值 (rvalue)  ：临时值，没有持久地址，不能取地址
// 将亡值 (xvalue)：即将被移动的右值（std::move 的返回值）
// 
// 核心口诀：
//   能取地址的是左值，不能取地址的是右值
//   有名字的是左值，没名字（临时）的是右值
// ============================================

int g_global = 100;

// 辅助函数：打印值类别（通过模板重载）
template<typename T>
void print_value_category(const T&) {
    std::cout << "  ✅ 是左值（const T& 可以绑定左值和右值）" << std::endl;
}

// 通过是否能绑定到非const左值引用判断左值
void accept_lvalue_ref(int& x) {
    std::cout << "  ✅ 是左值（可以绑定到 int&）" << std::endl;
}

void accept_rvalue_ref(int&& x) {
    std::cout << "  ✅ 是右值（可以绑定到 int&&）" << std::endl;
}

// 通用打印函数（使用重载）
void check_category(int& x) { 
    std::cout << "  🔵 左值（绑定到 int&）" << std::endl; 
}

void check_category(int&& x) { 
    std::cout << "  🔴 右值（绑定到 int&&）" << std::endl; 
}

// ============================================
// 主演示
// ============================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  左值 vs 右值 辨析" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // ---------- 1. 变量都是左值 ----------
    std::cout << "【1】变量是左值" << std::endl;
    int a = 10;
    check_category(a);           // ✅ 左值
    // check_category(10);       // ❌ 10是右值，不能绑定到 int&
    std::cout << std::endl;

    // ---------- 2. 字面量是右值 ----------
    std::cout << "【2】字面量是右值" << std::endl;
    check_category(42);          // ✅ 右值（纯右值）
    std::cout << std::endl;

    // ---------- 3. 表达式的结果 ----------
    std::cout << "【3】表达式的结果" << std::endl;
    int b = 20;
    check_category(a + b);       // ✅ 右值（临时结果）
    check_category(a = b);       // ✅ 赋值表达式返回左值（a的引用）
    std::cout << std::endl;

    // ---------- 4. 取地址 ----------
    std::cout << "【4】取地址测试" << std::endl;
    std::cout << "&a = " << &a << " ✅ 能取地址 → 左值" << std::endl;
    // std::cout << &(a + b);     // ❌ 不能取地址 → 右值
    std::cout << std::endl;

    // ---------- 5. 字符串字面量 ----------
    std::cout << "【5】字符串字面量是左值" << std::endl;
    const char* str = "hello";
    std::cout << "&str = " << &str << " (取的是指针变量的地址)" << std::endl;
    // "hello" 本身存储在常量区，有地址，是左值
    std::cout << std::endl;

    // ---------- 6. 函数返回值 ----------
    std::cout << "【6】函数返回值" << std::endl;
    auto get_value = []() -> int { return 100; };
    check_category(get_value());  // ✅ 右值（临时返回值）
    std::cout << std::endl;

    // ---------- 7. 将亡值 (xvalue) ----------
    std::cout << "【7】将亡值 (std::move)" << std::endl;
    int c = 50;
    check_category(std::move(c)); // ✅ 右值（将亡值）
    std::cout << "注意：std::move(c) 之后 c 仍然存在，但不应再使用" << std::endl;
    std::cout << "c 的值仍然是: " << c << "（移动后变量处于有效但未定义状态）" << std::endl;
    std::cout << std::endl;

    // ---------- 8. 总结表格 ----------
    std::cout << "========================================" << std::endl;
    std::cout << "  📊 总结表格" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "| 表达式示例          | 类别 | 能否取地址 |" << std::endl;
    std::cout << "|---------------------|------|-----------|" << std::endl;
    std::cout << "| 变量名 (int a)      | 左值 | ✅ 能     |" << std::endl;
    std::cout << "| 字面量 (42)         | 右值 | ❌ 不能   |" << std::endl;
    std::cout << "| a + b               | 右值 | ❌ 不能   |" << std::endl;
    std::cout << "| 赋值 a = b          | 左值 | ✅ 能     |" << std::endl;
    std::cout << "| 函数返回值          | 右值 | ❌ 不能   |" << std::endl;
    std::cout << "| std::move(x)        | 右值 | ❌ 不能   |" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}