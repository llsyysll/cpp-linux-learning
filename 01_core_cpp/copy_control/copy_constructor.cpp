#include <iostream>
#include <cstring>

// ✅ 正确示例：实现深拷贝
class DeepString {
public:
    // ============================================
    // 构造函数
    // ============================================
    // explicit 防止隐式转换：DeepString s = "Hello"; 会编译错误
    // 必须显式调用：DeepString s("Hello");
    explicit DeepString(const char* str = "") {
        size_t len = strlen(str);
        data_ = new char[len + 1];   // +1 给字符串结束符 '\0'
        strcpy(data_, str);
        std::cout << "[构造] data_ = " << data_ 
                  << " (地址: " << (void*)data_ << ")" << std::endl;
    }

    // ============================================
    // ✅ 拷贝构造函数（深拷贝）
    // ============================================
    // 参数必须是 const 引用，否则会无限递归
    // const: 不修改源对象
    // & : 引用传递，避免拷贝（拷贝构造里调用拷贝构造 → 死循环）
    DeepString(const DeepString& other) {
        // 步骤1：获取源字符串长度
        size_t len = strlen(other.data_);
        
        // 步骤2：在堆上分配新内存（独立于 other.data_）
        data_ = new char[len + 1];
        
        // 步骤3：复制数据（深拷贝的核心）
        strcpy(data_, other.data_);
        
        std::cout << "[拷贝构造] data_ = " << data_ 
                  << " (地址: " << (void*)data_ << ")" << std::endl;
    }

    // ============================================
    // 析构函数
    // ============================================
    ~DeepString() {
        std::cout << "[析构] 释放 data_ = " << data_ 
                  << " (地址: " << (void*)data_ << ")" << std::endl;
        delete[] data_;  // 释放自己的堆内存
    }

    // ============================================
    // 成员函数
    // ============================================
    void print() const {
        std::cout << data_ << std::endl;
    }

    // 返回 data_ 指针（只读）
    const char* c_str() const {
        return data_;
    }

    // 获取字符串长度
    size_t size() const {
        return strlen(data_);
    }

private:
    char* data_;  // 指向堆内存的指针
};

// ============================================
// 主函数测试
// ============================================
int main() {
    std::cout << "=== 深拷贝演示 ===" << std::endl;
    std::cout << std::endl;
    
    {
        // 1. 构造 s1
        DeepString s1("Hello");
        std::cout << "s1 内容: ";
        s1.print();
        std::cout << std::endl;
        
        // 2. 拷贝构造 s2（深拷贝）
        DeepString s2 = s1;  // 等价于 DeepString s2(s1);
        std::cout << "s2 内容: ";
        s2.print();
        std::cout << std::endl;
        
        // 3. 验证两个指针指向不同地址
        std::cout << "=== 地址验证 ===" << std::endl;
        std::cout << "s1.data_ 地址: " << (void*)s1.c_str() << std::endl;
        std::cout << "s2.data_ 地址: " << (void*)s2.c_str() << std::endl;
        
        if (s1.c_str() != s2.c_str()) {
            std::cout << "✅ 两个指针指向不同地址" << std::endl;
            std::cout << "✅ 深拷贝成功！" << std::endl;
        }
        std::cout << std::endl;
        
        // 4. 验证独立性（修改 s2 不影响 s1）
        // 注意：这里用 const_cast 只是为了演示，实际生产代码不要这样用
        // 更好的方式：提供 setter 方法
        std::cout << "=== 独立性验证 ===" << std::endl;
        char* p = const_cast<char*>(s2.c_str());
        strcpy(p, "World");
        std::cout << "修改 s2 内容为 'World'" << std::endl;
        std::cout << "s1 内容: ";
        s1.print();
        std::cout << "s2 内容: ";
        s2.print();
        std::cout << "✅ s1 不受影响，内存独立！" << std::endl;
        std::cout << std::endl;
        
        // 5. 离开作用域，s1 和 s2 分别析构
        std::cout << "=== 析构顺序（先构造的后析构） ===" << std::endl;
        // 注意：析构顺序是 s2 先析构，s1 后析构
        // 但这里我们手动控制输出顺序，用大括号已经做到了
    }
    // s2 先析构（后创建），s1 后析构（先创建）
    
    std::cout << std::endl;
    std::cout << "✅ 程序正常结束，无内存错误" << std::endl;
    return 0;
}