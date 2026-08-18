#include <iostream>
#include <cstring>

// ❌ 错误示例：未实现拷贝构造函数（使用编译器默认的浅拷贝）
class ShallowString {
public:
    // 构造函数：接收 C 风格字符串，在堆上分配内存
    ShallowString(const char* str = "") {
        data_ = new char[strlen(str) + 1];  // +1 给 '\0'
        strcpy(data_, str);
        std::cout << "[构造] data_ = " << data_ << " (地址: " << (void*)data_ << ")" << std::endl;
    }

    // 析构函数：释放堆内存
    ~ShallowString() {
        std::cout << "[析构] 释放 data_ = " << data_ << " (地址: " << (void*)data_ << ")" << std::endl;
        delete[] data_;
    }

    // ⚠️ 问题核心：这里没有定义拷贝构造函数！
    // 编译器会自动生成一个默认的拷贝构造函数，执行"浅拷贝"
    // 相当于：
    // ShallowString(const ShallowString& other) {
    //     data_ = other.data_;   // 只是复制指针值（地址）
    // }
    // 注意：这是编译器在后台生成的，代码里看不到！

    void print() const {
        std::cout << data_ << std::endl;
    }

private:
    char* data_;  // 指向堆内存的指针
};

int main() {
    std::cout << "=== 浅拷贝问题演示 ===" << std::endl;
    
    {
        ShallowString s1("Hello");
        // 此时内存状态：
        // s1.data_ → 堆地址 0x1000，内容 "Hello\0"
        
        ShallowString s2 = s1;  // ⚠️ 调用编译器生成的默认拷贝构造（浅拷贝）
        // 此时内存状态：
        // s1.data_ → 堆地址 0x1000，内容 "Hello\0"
        // s2.data_ → 堆地址 0x1000，内容 "Hello\0"  ← 两个指针指向同一块内存！
        
        // 离开作用域时：
        // 1. s2 析构 → delete[] 0x1000 → 释放内存
        // 2. s1 析构 → delete[] 0x1000 → 再次释放同一块内存 ❌ double free!
    }
    // 程序崩溃：double free or corruption (fasttop)
    
    return 0;
}