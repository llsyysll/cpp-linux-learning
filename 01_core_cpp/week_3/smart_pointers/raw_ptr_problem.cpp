#include <iostream>

class Resource {
public:
    explicit Resource(int id) : id_(id) { 
        std::cout << "Resource " << id_ << " acquired\n"; 
    }
    ~Resource() { 
        std::cout << "Resource " << id_ << " released\n"; 
    }
    void use() const { 
        std::cout << "Using Resource " << id_ << "\n"; 
    }
private:
    int id_;
};

// 问题1：内存泄漏（忘记 delete）
void test_leak() {
    std::cout << "\n--- test_leak ---\n";
    Resource* p = new Resource(1);
    p->use();
    // 忘记 delete p; → 析构不调用，资源永不释放
}

// 问题2：悬垂指针（野指针访问）
void test_dangling() {
    std::cout << "\n--- test_dangling ---\n";
    Resource* p = new Resource(2);
    delete p;
    p->use();  // 未定义行为：访问已释放的内存
}

// 问题3：重复释放（double free）
void test_double_free() {
    std::cout << "\n--- test_double_free ---\n";
    Resource* p = new Resource(3);
    delete p;
    delete p;  // 未定义行为：通常导致崩溃 (double free detected)
}

int main() {
    // 逐个取消注释运行，观察现象（建议配合 valgrind 或 gdb）
    // test_leak();    // 正常结束但未释放
    // test_dangling(); // 可能打印错误数据或崩溃
    // test_double_free(); // 必然崩溃（libc 检测到 double free）
    return 0;
}