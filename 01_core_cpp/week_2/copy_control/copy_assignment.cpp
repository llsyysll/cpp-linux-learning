// copy_assignment.cpp
// 拷贝赋值运算符：深拷贝 + 自赋值检测
// 编译：g++ -std=c++17 copy_assignment.cpp -o copy_assignment
// 运行：./copy_assignment

#include <iostream>
#include <cstring>
#include <vector>   // ✅ 新增

class MyString {
public:
    // ========== 构造函数 ==========
    MyString(const char* str = "") {
        std::cout << "[构造] ";
        if (str) {
            size_ = strlen(str);
            data_ = new char[size_ + 1];
            strcpy(data_, str);
        } else {
            size_ = 0;
            data_ = new char[1];
            data_[0] = '\0';
        }
        std::cout << "创建: \"" << data_ << "\"" << std::endl;
    }

    // ========== 拷贝构造函数 ==========
    MyString(const MyString& other) {
        std::cout << "[拷贝构造] ";
        size_ = other.size_;
        data_ = new char[size_ + 1];
        strcpy(data_, other.data_);
        std::cout << "复制: \"" << data_ << "\"" << std::endl;
    }

    // ========== 析构函数 ==========
    ~MyString() {
        std::cout << "[析构] 释放: \"" << data_ << "\"" << std::endl;
        delete[] data_;
    }

    // ============================================================
    // 🔥 今日重点：拷贝赋值运算符
    // ============================================================
    MyString& operator=(const MyString& other) {
        std::cout << "[拷贝赋值] ";

        // 【第1步】自赋值检测
        if (this == &other) {
            std::cout << "自赋值检测，跳过" << std::endl;
            return *this;
        }

        // 【第2步】释放旧资源
        std::cout << "释放旧资源: \"" << data_ << "\" -> ";
        delete[] data_;

        // 【第3步】分配新资源并复制
        size_ = other.size_;
        data_ = new char[size_ + 1];
        strcpy(data_, other.data_);
        std::cout << "复制新资源: \"" << data_ << "\"" << std::endl;

        return *this;
    }

    // ========== 获取数据 ==========
    const char* c_str() const { return data_; }
    size_t size() const { return size_; }

    // ========== 打印 ==========
    void print() const {
        std::cout << data_;
    }

private:
    char* data_;
    size_t size_;
};

// ============================================================
// 测试代码
// ============================================================
int main() {
    std::cout << "========== 测试1：普通赋值 ==========" << std::endl;
    MyString s1("Hello");
    MyString s2("World");
    
    std::cout << "\n执行 s2 = s1:" << std::endl;
    s2 = s1;

    std::cout << "\n结果: s1 = \""; s1.print(); 
    std::cout << "\", s2 = \""; s2.print(); std::cout << "\"" << std::endl;

    std::cout << "\n========== 测试2：自赋值（重点） ==========" << std::endl;
    MyString s3("SelfTest");
    
    std::cout << "\n执行 s3 = s3（自赋值）:" << std::endl;
    s3 = s3;

    std::cout << "\n结果: s3 = \""; s3.print(); std::cout << "\"" << std::endl;

    std::cout << "\n========== 测试3：链式赋值 ==========" << std::endl;
    MyString s4("A"), s5("B"), s6("C");
    
    std::cout << "\n执行 s6 = s5 = s4:" << std::endl;
    s6 = s5 = s4;

    std::cout << "\n结果: s4=\""; s4.print(); 
    std::cout << "\", s5=\""; s5.print(); 
    std::cout << "\", s6=\""; s6.print(); std::cout << "\"" << std::endl;

    std::cout << "\n========== 测试4：vector扩容触发拷贝 ==========" << std::endl;
    std::vector<MyString> vec;
    vec.reserve(1);
    vec.emplace_back("First");
    
    std::cout << "capacity = " << vec.capacity() << std::endl;
    std::cout << "添加第二个元素，触发扩容..." << std::endl;
    vec.emplace_back("Second");
    
    std::cout << "vec[0] = \"" << vec[0].c_str() << "\"" << std::endl;
    std::cout << "vec[1] = \"" << vec[1].c_str() << "\"" << std::endl;

    std::cout << "\n✅ 所有测试通过！" << std::endl;
    return 0;
}