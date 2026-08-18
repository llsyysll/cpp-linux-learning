// shallow_copy_problem.cpp
// 浅拷贝导致的问题：double free、悬空指针
// 编译：g++ -std=c++17 shallow_copy_problem.cpp -o shallow_copy_problem
// 运行：./shallow_copy_problem

#include <iostream>
#include <cstring>

// ❌ 错误示例：未实现拷贝构造函数（使用编译器默认的浅拷贝）
class ShallowString {
public:
    ShallowString(const char* str = "") {
        data_ = new char[strlen(str) + 1];
        strcpy(data_, str);
        std::cout << "[构造] data_ = " << data_ << std::endl;
    }

    ~ShallowString() {
        std::cout << "[析构] 释放 data_ = " << data_ << std::endl;
        delete[] data_;
    }

    // 编译器默认生成的拷贝构造 = 浅拷贝（逐字节复制指针值）
    // 相当于：data_ = other.data_（两个指针指向同一块内存）

    void print() const {
        std::cout << data_ << std::endl;
    }

private:
    char* data_;
};

int main() {
    std::cout << "=== 浅拷贝问题演示 ===" << std::endl;
    
    {
        ShallowString s1("Hello");
        ShallowString s2 = s1;  // 调用默认拷贝构造（浅拷贝）
        
        // s1.data_ 和 s2.data_ 指向同一块内存
        // s1 析构时释放了内存
        // s2 析构时再次释放 → double free ❌
    }
    // 程序崩溃：double free or corruption
    
    return 0;
}