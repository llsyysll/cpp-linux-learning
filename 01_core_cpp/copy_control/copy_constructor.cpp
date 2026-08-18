// copy_constructor.cpp
// 实现深拷贝拷贝构造函数
// 编译：g++ -std=c++17 copy_constructor.cpp -o copy_constructor
// 运行：./copy_constructor

#include <iostream>
#include <cstring>

// ✅ 正确示例：实现深拷贝
class DeepString {
public:
    // 构造函数
    explicit DeepString(const char* str = "") {
        size_t len = strlen(str);
        data_ = new char[len + 1];
        strcpy(data_, str);
        std::cout << "[构造] data_ = " << data_ << " (地址: " << (void*)data_ << ")" << std::endl;
    }

    // ✅ 拷贝构造函数（深拷贝）
    DeepString(const DeepString& other) {
        size_t len = strlen(other.data_);
        data_ = new char[len + 1];
        strcpy(data_, other.data_);
        std::cout << "[拷贝构造] data_ = " << data_ << " (地址: " << (void*)data_ << ")" << std::endl;
    }

    // 析构函数
    ~DeepString() {
        std::cout << "[析构] 释放 data_ = " << data_ << " (地址: " << (void*)data_ << ")" << std::endl;
        delete[] data_;
    }

    void print() const {
        std::cout << data_ << std::endl;
    }

    const char* c_str() const {
        return data_;
    }

private:
    char* data_;
};

int main() {
    std::cout << "=== 深拷贝演示 ===" << std::endl;
    
    {
        DeepString s1("Hello");
        DeepString s2 = s1;  // 调用拷贝构造（深拷贝）
        
        std::cout << "s1.data_ 地址: " << (void*)s1.c_str() << std::endl;
        std::cout << "s2.data_ 地址: " << (void*)s2.c_str() << std::endl;
        std::cout << "两个指针指向不同地址 ✅" << std::endl;
        
        // s1 和 s2 各自拥有独立内存，不会 double free
    }
    
    std::cout << "\n✅ 程序正常结束，无内存错误" << std::endl;
    return 0;
}