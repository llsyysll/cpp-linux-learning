#include <iostream>

template<typename T>
class AutoPtr {
public:
    explicit AutoPtr(T* ptr = nullptr) : ptr_(ptr) {
        std::cout << "AutoPtr ctor, ptr = " << (ptr ? "non-null" : "null") << "\n";
    }

    ~AutoPtr() {
        std::cout << "AutoPtr dtor, deleting " << (ptr_ ? "resource" : "null") << "\n";
        delete ptr_;
    }

    // ⚠️ 拷贝构造：转移所有权（原指针置空）
    AutoPtr(AutoPtr& other) : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
        std::cout << "AutoPtr copy ctor (steal ownership)\n";
    }

    // ⚠️ 拷贝赋值：释放自己，接管对方，置空对方
    AutoPtr& operator=(AutoPtr& other) {
        std::cout << "AutoPtr copy assignment\n";
        if (this != &other) {
            delete ptr_;          // 释放当前资源
            ptr_ = other.ptr_;    // 接管对方
            other.ptr_ = nullptr; // 对方失去所有权
        }
        return *this;
    }

    // 指针模拟操作
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    
    // 仅用于观察裸指针地址（生产环境不暴露）
    T* get() const { return ptr_; }

private:
    T* ptr_;
};

// 测试资源
class TestResource {
public:
    explicit TestResource(int id) : id_(id) { 
        std::cout << "  -> TestResource " << id_ << " created\n"; 
    }
    ~TestResource() { 
        std::cout << "  -> TestResource " << id_ << " destroyed\n"; 
    }
    void greet() const { 
        std::cout << "  Hello from Resource " << id_ << "\n"; 
    }
private:
    int id_;
};

int main() {
    std::cout << "=== 场景1：正常生命周期 ===\n";
    {
        AutoPtr<TestResource> p1(new TestResource(1));
        p1->greet();
    } // p1析构，自动释放

    std::cout << "\n=== 场景2：拷贝构造导致原指针悬空 ===\n";
    {
        AutoPtr<TestResource> p2(new TestResource(2));
        AutoPtr<TestResource> p3(p2); // p2 失去所有权
        
        std::cout << "p2.get() = " << p2.get() << " (现在为 nullptr)\n";
        // p2->greet(); // 如果取消注释，运行期崩溃（访问空指针）
        p3->greet();    // p3 正常工作
    } // p3析构释放，p2析构无操作

    std::cout << "\n=== 场景3：拷贝赋值同样危险 ===\n";
    {
        AutoPtr<TestResource> p4(new TestResource(3));
        AutoPtr<TestResource> p5(new TestResource(4));
        
        p4 = p5; // p4 先释放自己(3)，再接管 p5(4)，p5 置空
        std::cout << "p5.get() = " << p5.get() << " (现在为 nullptr)\n";
        p4->greet(); // 正常工作
    }

    std::cout << "\n=== 场景4：容器中的灾难（经典 auto_ptr 被禁原因） ===\n";
    // 如果将 AutoPtr 放入 vector，拷贝时会导致元素被置空，严重破坏语义
    // 明天我们用 unique_ptr 的移动语义完美解决此问题

    return 0;
}