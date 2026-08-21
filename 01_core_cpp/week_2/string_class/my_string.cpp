// my_string.cpp
// 第2周核心产出：完整的MyString类（深拷贝 + 移动语义）
// 编译：g++ -std=c++17 -Wall -g my_string.cpp -o my_string
// 运行：./my_string

#include <iostream>
#include <cstring>
#include <utility>

class MyString {
public:
    // ========== 1. 构造函数 ==========
    
    // 默认构造（空字符串）
    MyString() : data_(nullptr), len_(0) {
        std::cout << "[默认构造] 空字符串" << std::endl;
    }

    // 从 C 风格字符串构造（普通构造函数）
    MyString(const char* str) : len_(str ? strlen(str) : 0) {
        std::cout << "[构造] 从 C 字符串: \"" << (str ? str : "null") << "\"" << std::endl;
        if (str) {
            data_ = new char[len_ + 1];
            strcpy(data_, str);
        } else {
            data_ = new char[1];
            data_[0] = '\0';
        }
    }

    // ========== 2. 拷贝控制（深拷贝） ==========
    
    // 拷贝构造函数
    MyString(const MyString& other) : len_(other.len_) {
        std::cout << "[拷贝构造] 深拷贝: \"" << (other.data_ ? other.data_ : "") << "\"" << std::endl;
        if (other.data_) {
            data_ = new char[len_ + 1];
            strcpy(data_, other.data_);
        } else {
            data_ = new char[1];
            data_[0] = '\0';
        }
    }

    // 拷贝赋值运算符（含自赋值检测）
    MyString& operator=(const MyString& other) {
        std::cout << "[拷贝赋值] 从 \"" << (other.data_ ? other.data_ : "") << "\"" << std::endl;
        
        if (this == &other) {  // ✅ 自赋值检测
            std::cout << "  -> 检测到自赋值，跳过" << std::endl;
            return *this;
        }

        // 释放旧资源
        delete[] data_;

        // 分配新资源并拷贝
        len_ = other.len_;
        if (other.data_) {
            data_ = new char[len_ + 1];
            strcpy(data_, other.data_);
        } else {
            data_ = new char[1];
            data_[0] = '\0';
        }
        return *this;
    }

    // ========== 3. 移动语义（转移资源所有权） ==========
    
    // 移动构造函数
    MyString(MyString&& other) noexcept 
        : data_(other.data_), len_(other.len_) {
        std::cout << "[移动构造] 窃取资源: \"" << (other.data_ ? other.data_ : "") << "\"" << std::endl;
        // 将源对象置为有效但未定义状态（防止析构时 double free）
        other.data_ = nullptr;
        other.len_ = 0;
    }

    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        std::cout << "[移动赋值] 窃取资源: \"" << (other.data_ ? other.data_ : "") << "\"" << std::endl;
        
        if (this == &other) {
            return *this;
        }

        // 释放当前对象的旧资源
        delete[] data_;

        // 接管 other 的资源
        data_ = other.data_;
        len_ = other.len_;

        // 将 other 置空
        other.data_ = nullptr;
        other.len_ = 0;

        return *this;
    }

    // ========== 4. 析构函数 ==========
    ~MyString() {
        std::cout << "[析构] 释放资源: \"" << (data_ ? data_ : "nullptr") << "\"" << std::endl;
        delete[] data_;
        data_ = nullptr;
        len_ = 0;
    }

    // ========== 5. 辅助方法 ==========
    size_t size() const { return len_; }
    const char* c_str() const { return data_ ? data_ : ""; }

    void print() const {
        std::cout << "  [内容] = \"" << (data_ ? data_ : "") << "\", 长度 = " << len_ << std::endl;
    }

private:
    char* data_;   // 动态分配的字符数组
    size_t len_;   // 字符串长度（不含 '\0'）

    // 友元函数：重载 << 便于输出
    friend std::ostream& operator<<(std::ostream& os, const MyString& s);
};

std::ostream& operator<<(std::ostream& os, const MyString& s) {
    os << (s.data_ ? s.data_ : "");
    return os;
}

// ============================================
// 测试用例（涵盖所有场景）
// ============================================
int main() {
    std::cout << "========== 第1阶段：构造 ==========" << std::endl;
    MyString s1;                     // 默认构造
    MyString s2("Hello");            // 普通构造
    MyString s3("World");

    std::cout << "\n========== 第2阶段：拷贝构造 ==========" << std::endl;
    MyString s4(s2);                 // 拷贝构造

    std::cout << "\n========== 第3阶段：拷贝赋值 ==========" << std::endl;
    MyString s5;
    s5 = s3;                         // 拷贝赋值

    std::cout << "\n========== 第4阶段：自赋值测试 ==========" << std::endl;
    s5 = s5;                         // 自赋值（必须安全）

    std::cout << "\n========== 第5阶段：移动构造 ==========" << std::endl;
    MyString s6(std::move(s2));      // 移动构造（s2 被掏空）

    std::cout << "\n========== 第6阶段：移动赋值 ==========" << std::endl;
    MyString s7;
    s7 = std::move(s3);              // 移动赋值（s3 被掏空）

    std::cout << "\n========== 第7阶段：打印最终结果 ==========" << std::endl;
    std::cout << "s1: " << s1 << std::endl;
    std::cout << "s2 (移动后): " << s2 << " (应为空)" << std::endl;
    std::cout << "s3 (移动后): " << s3 << " (应为空)" << std::endl;
    std::cout << "s4 (拷贝自s2): " << s4 << std::endl;
    std::cout << "s5: " << s5 << std::endl;
    std::cout << "s6 (移动自s2): " << s6 << std::endl;
    std::cout << "s7 (移动自s3): " << s7 << std::endl;

    std::cout << "\n========== 析构顺序（栈上后进先出） ==========" << std::endl;
    return 0;
}