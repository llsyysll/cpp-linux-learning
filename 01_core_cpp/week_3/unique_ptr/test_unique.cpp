#include "MyUniquePtr.h"
#include <vector>

class TestObj {
public:
    int id;
    explicit TestObj(int i) : id(i) {
        std::cout << "TestObj " << id << " constructed" << std::endl;
    }
    ~TestObj() {
        std::cout << "TestObj " << id << " destructed" << std::endl;
    }
    void print() const {
        std::cout << "TestObj id = " << id << std::endl;
    }
};

int main() {
    std::cout << "=== 1. 创建 unique_ptr ===\n";
    MyUniquePtr<TestObj> p1(new TestObj(100));
    p1->print();   // 使用 operator->
    (*p1).print(); // 使用 operator*

    std::cout << "\n=== 2. 移动构造（转移所有权） ===\n";
    MyUniquePtr<TestObj> p2 = std::move(p1);  // 移动构造
    if (!p1) {
        std::cout << "p1 已为空 (nullptr)" << std::endl;
    }
    p2->print();

    std::cout << "\n=== 3. 移动赋值（释放旧资源，接管新资源） ===\n";
    MyUniquePtr<TestObj> p3(new TestObj(200));
    p3 = std::move(p2);  // 移动赋值：p3 释放 200，接管 p2 的 100
    if (!p2) {
        std::cout << "p2 已为空" << std::endl;
    }
    p3->print();

    std::cout << "\n=== 4. 放入容器（只能移动，不能拷贝） ===\n";
    std::vector<MyUniquePtr<TestObj>> vec;
    vec.push_back(std::move(p3));  // 必须显式 move
    if (!p3) {
        std::cout << "p3 已为空（资源已被 vec 接管）" << std::endl;
    }
    vec[0]->print();

    std::cout << "\n=== 作用域结束，自动析构 ===\n";
    return 0;
}