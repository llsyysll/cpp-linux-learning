// raii_comprehensive.cpp
// 综合练习：RAII管理动态数组 + 自定义RAII类
// 编译：g++ -std=c++17 raii_comprehensive.cpp -o raii_comprehensive
// 运行：./raii_comprehensive

#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>

// ========== 练习1：RAII管理动态数组 ==========
class IntArrayRAII {
private:
    int* data_;
    size_t size_;
    
public:
    // 构造函数：分配内存
    explicit IntArrayRAII(size_t size) 
        : data_(new int[size]), size_(size) {
        std::cout << "📦 分配 " << size << " 个int (RAII)\n";
        // 初始化为0
        for (size_t i = 0; i < size; ++i) {
            data_[i] = 0;
        }
    }
    
    // 析构函数：释放内存
    ~IntArrayRAII() {
        if (data_) {
            std::cout << "🗑️  释放 " << size_ << " 个int (RAII)\n";
            delete[] data_;
            data_ = nullptr;
        }
    }
    
    // 拷贝构造（深拷贝）
    IntArrayRAII(const IntArrayRAII& other) 
        : data_(new int[other.size_]), size_(other.size_) {
        std::cout << "📋 拷贝构造 (深拷贝)\n";
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    
    // 拷贝赋值（深拷贝）
    IntArrayRAII& operator=(const IntArrayRAII& other) {
        if (this != &other) {
            std::cout << "📋 拷贝赋值 (深拷贝)\n";
            // 释放旧资源
            delete[] data_;
            // 分配新资源并拷贝
            data_ = new int[other.size_];
            size_ = other.size_;
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }
    
    // 移动构造
    IntArrayRAII(IntArrayRAII&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        std::cout << "🚚 移动构造\n";
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    // 移动赋值
    IntArrayRAII& operator=(IntArrayRAII&& other) noexcept {
        if (this != &other) {
            std::cout << "🚚 移动赋值\n";
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    // 访问元素
    int& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("索引越界");
        }
        return data_[index];
    }
    
    const int& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("索引越界");
        }
        return data_[index];
    }
    
    size_t size() const { return size_; }
    
    // 打印数组
    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << data_[i];
            if (i < size_ - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

// ========== 练习2：RAII管理自定义资源（Socket模拟） ==========
class SocketRAII {
private:
    int socket_fd_;
    bool is_open_;
    
public:
    SocketRAII() : socket_fd_(-1), is_open_(false) {
        // 模拟创建socket
        socket_fd_ = 100 + rand() % 1000;
        is_open_ = true;
        std::cout << "🔌 Socket创建: fd=" << socket_fd_ << "\n";
    }
    
    ~SocketRAII() {
        close_socket();
    }
    
    // 禁止拷贝
    SocketRAII(const SocketRAII&) = delete;
    SocketRAII& operator=(const SocketRAII&) = delete;
    
    // 允许移动
    SocketRAII(SocketRAII&& other) noexcept 
        : socket_fd_(other.socket_fd_), is_open_(other.is_open_) {
        other.socket_fd_ = -1;
        other.is_open_ = false;
    }
    
    SocketRAII& operator=(SocketRAII&& other) noexcept {
        if (this != &other) {
            close_socket();
            socket_fd_ = other.socket_fd_;
            is_open_ = other.is_open_;
            other.socket_fd_ = -1;
            other.is_open_ = false;
        }
        return *this;
    }
    
    void close_socket() {
        if (is_open_ && socket_fd_ != -1) {
            std::cout << "🔌 Socket关闭: fd=" << socket_fd_ << "\n";
            // 模拟关闭socket
            socket_fd_ = -1;
            is_open_ = false;
        }
    }
    
    void send_data(const std::string& data) const {
        if (!is_open_) {
            throw std::runtime_error("Socket未打开");
        }
        std::cout << "📤 发送数据: " << data << " (fd=" << socket_fd_ << ")\n";
    }
    
    int fd() const { return socket_fd_; }
    bool is_open() const { return is_open_; }
};

// ========== 练习3：RAII + 异常安全测试 ==========
class ResourceA {
public:
    ResourceA() { std::cout << "🔷 资源A创建\n"; }
    ~ResourceA() { std::cout << "🔷 资源A销毁\n"; }
};

class ResourceB {
public:
    ResourceB() { 
        std::cout << "🔶 资源B创建\n";
        // 模拟构造失败
        // throw std::runtime_error("B构造失败");
    }
    ~ResourceB() { std::cout << "🔶 资源B销毁\n"; }
};

class CompositeRAII {
private:
    std::unique_ptr<ResourceA> ra_;  // 使用RAII管理
    std::unique_ptr<ResourceB> rb_;
    
public:
    CompositeRAII() 
        : ra_(std::make_unique<ResourceA>()),
          rb_(std::make_unique<ResourceB>()) {
        std::cout << "📦 Composite创建\n";
    }
    
    ~CompositeRAII() {
        std::cout << "🗑️  Composite销毁\n";
        // unique_ptr会自动释放资源
    }
};

void exception_safety_test() {
    std::cout << "\n=== 异常安全测试 ===\n";
    try {
        CompositeRAII composite;
        std::cout << "✅ Composite工作正常\n";
        // 如果ResourceB构造失败，ResourceA会如何？
        // 答案：ResourceA已经被unique_ptr管理，会自动释放
    } catch (const std::exception& e) {
        std::cout << "❌ 异常捕获: " << e.what() << "\n";
        // 即使发生异常，已构造的资源也会被正确释放
    }
}

// ========== 主函数 ==========
int main() {
    std::cout << "========== RAII 综合练习 ==========\n\n";
    
    // ---------- 练习1：动态数组 ----------
    std::cout << "【练习1：RAII管理动态数组】\n";
    {
        IntArrayRAII arr(5);
        for (size_t i = 0; i < arr.size(); ++i) {
            arr[i] = static_cast<int>(i * i);
        }
        arr.print();
        
        // 拷贝构造测试
        IntArrayRAII arr2 = arr;
        arr2[0] = 100;
        std::cout << "arr: "; arr.print();
        std::cout << "arr2: "; arr2.print();
        
        // 移动构造测试
        IntArrayRAII arr3 = std::move(arr2);
        std::cout << "arr3: "; arr3.print();
        // arr2现在是空
        std::cout << "arr2 size: " << arr2.size() << "\n";
    }  // 所有数组自动释放
    
    // ---------- 练习2：Socket ----------
    std::cout << "\n【练习2：RAII管理Socket】\n";
    {
        SocketRAII sock;
        sock.send_data("Hello, Server!");
        
        {
            SocketRAII sock2 = std::move(sock);
            sock2.send_data("从移动后的socket发送");
            // sock2离开作用域自动关闭
        }
        // sock现在是空（被移动了）
    }
    
    // ---------- 练习3：异常安全 ----------
    exception_safety_test();
    
    // ---------- 实际应用：对比 ----------
    std::cout << "\n【实际应用对比：手动new/delete vs RAII】\n";
    
    // ❌ 手动管理（危险）
    std::cout << "❌ 手动管理:\n";
    int* p = new int(42);
    std::cout << "p = " << *p << "\n";
    // 如果这里发生异常或提前return...
    delete p;  // 容易忘记
    
    // ✅ RAII管理（安全）
    std::cout << "✅ RAII管理:\n";
    {
        std::unique_ptr<int> up = std::make_unique<int>(42);
        std::cout << "up = " << *up << "\n";
        // 自动释放
    }
    
    std::cout << "\n✅ RAII综合练习完成！\n";
    return 0;
}