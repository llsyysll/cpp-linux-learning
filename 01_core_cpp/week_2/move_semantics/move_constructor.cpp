// move_constructor.cpp
// 移动构造 + 移动赋值 完整实现与测试
// 编译：g++ -std=c++17 -O2 move_constructor.cpp -o move_constructor
// 运行：./move_constructor

#include <iostream>
#include <cstring>
#include <vector>
#include <chrono>

class String {
public:
    // ========== 构造函数 ==========
    String() : data_(nullptr), size_(0) {
        std::cout << "[构造] 空字符串" << std::endl;
    }

    String(const char* str) : size_(strlen(str)) {
        data_ = new char[size_ + 1];
        strcpy(data_, str);
        std::cout << "[构造] 从 C 字符串: " << str << std::endl;
    }

    // ========== 拷贝构造（深拷贝） ==========
    String(const String& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        strcpy(data_, other.data_);
        std::cout << "[拷贝构造] 深拷贝: " << other.data_ << std::endl;
    }

    // ========== 拷贝赋值（深拷贝） ==========
    String& operator=(const String& other) {
        std::cout << "[拷贝赋值] 深拷贝: " << other.data_ << std::endl;
        if (this == &other) {
            return *this;   // 自赋值检测
        }
        delete[] data_;
        size_ = other.size_;
        data_ = new char[size_ + 1];
        strcpy(data_, other.data_);
        return *this;
    }

    // ========== 移动构造（⭐ 核心） ==========
    String(String&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        std::cout << "[移动构造] 窃取资源: " << other.data_ << std::endl;
        // 把源对象置空，防止析构时 double free
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // ========== 移动赋值（⭐ 核心） ==========
    String& operator=(String&& other) noexcept {
        std::cout << "[移动赋值] 窃取资源: " << other.data_ << std::endl;
        if (this == &other) {
            return *this;
        }
        // 释放当前资源
        delete[] data_;
        // 窃取资源
        data_ = other.data_;
        size_ = other.size_;
        // 源对象置空
        other.data_ = nullptr;
        other.size_ = 0;
        return *this;
    }

    // ========== 析构函数 ==========
    ~String() {
        if (data_) {
            std::cout << "[析构] 释放: " << data_ << std::endl;
            delete[] data_;
        } else {
            std::cout << "[析构] 空指针（已移走）" << std::endl;
        }
    }

    // ========== 工具方法 ==========
    size_t size() const { return size_; }
    const char* c_str() const { return data_ ? data_ : ""; }

    // 友元输出
    friend std::ostream& operator<<(std::ostream& os, const String& s) {
        os << s.c_str();
        return os;
    }

private:
    char* data_;
    size_t size_;
};

// ============================================
// 测试代码
// ============================================

// 1. 基本移动语义测试
void test_basic_move() {
    std::cout << "\n========== 基本移动语义 ==========" << std::endl;
    
    String s1("Hello");
    String s2 = std::move(s1);   // 移动构造
    
    std::cout << "s1 现在是: " << s1 << " (size=" << s1.size() << ")" << std::endl;
    std::cout << "s2 现在是: " << s2 << " (size=" << s2.size() << ")" << std::endl;
    // s1 已经为空，不能再使用其内容，但可以安全析构
}

// 2. 移动赋值测试
void test_move_assignment() {
    std::cout << "\n========== 移动赋值 ==========" << std::endl;
    
    String s1("World");
    String s2("Temp");
    s2 = std::move(s1);   // 移动赋值
    
    std::cout << "s1 现在是: " << s1 << std::endl;
    std::cout << "s2 现在是: " << s2 << std::endl;
}

// 3. 拷贝 vs 移动 性能对比（通过 vector 扩容演示）
void test_performance_comparison() {
    std::cout << "\n========== 拷贝 vs 移动 性能对比 ==========" << std::endl;
    std::cout << "通过 vector 插入 100000 个字符串观察拷贝/移动次数" << std::endl;
    
    std::vector<String> vec;
    // 提前 reserve 避免频繁扩容干扰，我们关注的是插入时的构造方式
    
    std::cout << "\n--- 使用拷贝（传入左值） ---" << std::endl;
    {
        std::vector<String> v;
        v.reserve(10);  // 小容量方便观察
        String temp("CopyMe");
        for (int i = 0; i < 3; ++i) {
            v.push_back(temp);   // 拷贝构造（因为 temp 是左值）
        }
        std::cout << "共插入 3 次，调用 3 次拷贝构造" << std::endl;
    }
    
    std::cout << "\n--- 使用移动（传入右值） ---" << std::endl;
    {
        std::vector<String> v;
        v.reserve(10);
        for (int i = 0; i < 3; ++i) {
            v.push_back(String("MoveMe"));   // 移动构造（临时对象是右值）
        }
        std::cout << "共插入 3 次，调用 3 次移动构造（无深拷贝）" << std::endl;
    }
}

// 4. std::move 的本质测试
void test_move_essence() {
    std::cout << "\n========== std::move 的本质 ==========" << std::endl;
    
    String a("Apple");
    String b("Banana");
    
    std::cout << "std::move 只是把左值转成右值引用，并不移动任何东西" << std::endl;
    std::cout << "真正的移动发生在 移动构造 或 移动赋值 中" << std::endl;
    
    // std::move(a) 只是返回 String&&，此时还没有移动
    String c(std::move(a));   // 这里才真正移动构造
    // 现在 a 已经被移走
    
    std::cout << "a 已被移走: " << a << std::endl;
    std::cout << "c 是: " << c << std::endl;
}

// ============================================
// 主函数
// ============================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  移动构造 + 移动赋值 完整测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    test_basic_move();
    test_move_assignment();
    test_performance_comparison();
    test_move_essence();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  ✅ 所有测试完成" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}