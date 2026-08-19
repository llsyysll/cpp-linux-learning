// std_move_demo.cpp
// std::move 的本质：无条件将左值转为右值引用
// 编译：g++ -std=c++17 std_move_demo.cpp -o std_move_demo
// 运行：./std_move_demo

#include <iostream>
#include <utility>
#include <vector>
#include <string>

// ============================================
// 手动实现 std::move（核心本质）
// ============================================
template<typename T>
typename std::remove_reference<T>::type&&
my_move(T&& param) {
    // 将任何类型转换为右值引用
    return static_cast<typename std::remove_reference<T>::type&&>(param);
}

// ============================================
// 用于演示的类（打印拷贝/移动）
// ============================================
class Demo {
public:
    Demo() : data_(new int(0)) {
        std::cout << "  [构造] Demo()" << std::endl;
    }

    explicit Demo(int val) : data_(new int(val)) {
        std::cout << "  [构造] Demo(" << val << ")" << std::endl;
    }

    // 拷贝构造（深拷贝）
    Demo(const Demo& other) : data_(new int(*other.data_)) {
        std::cout << "  [拷贝构造] 深拷贝 data = " << *data_ << std::endl;
    }

    // 移动构造（窃取资源）
    Demo(Demo&& other) noexcept : data_(other.data_) {
        other.data_ = nullptr;
        std::cout << "  [移动构造] 窃取资源" << std::endl;
    }

    // 拷贝赋值
    Demo& operator=(const Demo& other) {
        if (this != &other) {
            delete data_;
            data_ = new int(*other.data_);
            std::cout << "  [拷贝赋值] 深拷贝 data = " << *data_ << std::endl;
        }
        return *this;
    }

    // 移动赋值
    Demo& operator=(Demo&& other) noexcept {
        if (this != &other) {
            delete data_;
            data_ = other.data_;
            other.data_ = nullptr;
            std::cout << "  [移动赋值] 窃取资源" << std::endl;
        }
        return *this;
    }

    ~Demo() {
        if (data_) {
            std::cout << "  [析构] 释放 data = " << *data_ << std::endl;
            delete data_;
        } else {
            std::cout << "  [析构] data 为空（已被移动）" << std::endl;
        }
    }

    int get() const { return data_ ? *data_ : -1; }

private:
    int* data_;
};

// ============================================
// 主演示
// ============================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  std::move 的本质演示" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // ---------- 1. 拷贝 vs 移动 ----------
    std::cout << "【1】拷贝 vs 移动" << std::endl;
    Demo d1(100);
    Demo d2 = d1;              // 调用拷贝构造
    Demo d3 = std::move(d1);   // 调用移动构造
    std::cout << "d1.get() = " << d1.get() << " (已被移动，为空)" << std::endl;
    std::cout << std::endl;

    // ---------- 2. std::move 用在 vector 上 ----------
    std::cout << "【2】std::move 减少 vector 拷贝" << std::endl;
    std::vector<std::string> v1;
    v1.push_back("hello");
    v1.push_back("world");

    // 不使用 move：拷贝
    std::vector<std::string> v2 = v1;
    std::cout << "v2[0] = " << v2[0] << " (拷贝)" << std::endl;

    // 使用 move：移动（避免深拷贝）
    std::vector<std::string> v3 = std::move(v1);
    std::cout << "v3[0] = " << v3[0] << " (移动)" << std::endl;
    std::cout << "v1.size() = " << v1.size() << " (被移动后为空)" << std::endl;
    std::cout << std::endl;

    // ---------- 3. std::move 的真相 ----------
    std::cout << "【3】std::move 的真相" << std::endl;
    std::cout << "std::move 本质上就是一个 static_cast" << std::endl;
    std::cout << "它只是把类型转换成右值引用，本身不移动任何东西" << std::endl;
    std::cout << "真正移动的是移动构造函数和移动赋值运算符" << std::endl;

    // 使用 my_move（自己实现的版本）
    Demo d4(200);
    Demo d5 = my_move(d4);
    std::cout << "d4.get() = " << d4.get() << std::endl;
    std::cout << std::endl;

    // ---------- 4. 什么时候用 std::move ----------
    std::cout << "【4】何时使用 std::move" << std::endl;
    std::cout << "✅ 需要将左值转为右值时：std::move(lvalue)" << std::endl;
    std::cout << "✅ 移动语义场景：构造函数、赋值运算符" << std::endl;
    std::cout << "❌ 不要对 const 对象使用 move（会退化为拷贝）" << std::endl;
    std::cout << "❌ 不要对被移动后的对象做操作（只保证可析构）" << std::endl;
    std::cout << "❌ 不要对基本类型（int/char等）使用 move（没意义）" << std::endl;

    std::cout << "\n========================================" << std::endl;
    std::cout << "  ✅ 演示完成" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}