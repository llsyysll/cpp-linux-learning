// week1_review.cpp
// 第1周复习：const/引用/内存管理/RAII（不涉及拷贝/移动）
// 编译：g++ -std=c++17 week1_review.cpp -o week1_review
// 运行：./week1_review

#include <iostream>
#include <cstdio>
#include <stdexcept>

// ========== 1. const 用法复习 ==========

// ① const修饰变量
const int MAX_BUFFER = 1024;

// ② const修饰指针
void const_pointer_demo() {
    int a = 10, b = 20;
    
    // 指向const的指针：指向的内容不可变
    const int* p1 = &a;
    // *p1 = 30;  // ❌ 编译错误：不能修改指向的内容
    p1 = &b;      // ✅ 可以改变指向
    
    // const指针：指针本身不可变
    int* const p2 = &a;
    *p2 = 30;     // ✅ 可以修改指向的内容
    // p2 = &b;   // ❌ 编译错误：不能改变指向
    
    // 两者都const
    const int* const p3 = &a;
    // *p3 = 30;  // ❌ 
    // p3 = &b;   // ❌
    
    std::cout << "✅ const指针演示完成\n";
}

// ③ const修饰成员函数
class Counter {
private:
    int value_;
    mutable int cache_hit_;  // mutable：const成员函数也可修改
    
public:
    Counter(int v = 0) : value_(v), cache_hit_(0) {}
    
    // const成员函数：不修改对象状态（除了mutable）
    int getValue() const {
        // value_++;  // ❌ 编译错误：const函数不能修改普通成员
        cache_hit_++;  // ✅ mutable成员可以修改
        return value_;
    }
    
    // 非const成员函数：可以修改
    void increment() {
        value_++;
    }
    
    // const重载：const对象调用const版本
    void print() const {
        std::cout << "const print: value=" << value_ 
                  << ", cache=" << cache_hit_ << "\n";
    }
    
    void print() {
        std::cout << "non-const print: value=" << value_ 
                  << ", cache=" << cache_hit_ << "\n";
    }
};

void test_const_member() {
    Counter c1(100);
    const Counter c2(200);
    
    c1.print();  // 调用非const版本
    c2.print();  // 调用const版本
    
    std::cout << "c2.getValue() = " << c2.getValue() << "\n";
}

// ④ const作为返回值
const int& getConstRef() {
    static int value = 999;
    return value;  // 返回const引用，调用者不能修改
}

void test_const_return() {
    const int& ref = getConstRef();
    // ref = 100;  // ❌ 编译错误：不能修改const引用
    std::cout << "getConstRef() = " << ref << "\n";
}

// ========== 2. 引用与指针复习 ==========

// 引用参数：交换两个值
void swap_with_ref(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// 指针参数：交换两个值
void swap_with_ptr(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 引用作为参数：避免拷贝
void printArray(const int* arr, int size) {
    std::cout << "[";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i];
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

void reference_demo() {
    int x = 10, y = 20;
    std::cout << "交换前: x=" << x << ", y=" << y << "\n";
    
    swap_with_ref(x, y);
    std::cout << "交换后(引用): x=" << x << ", y=" << y << "\n";
    
    swap_with_ptr(&x, &y);
    std::cout << "交换后(指针): x=" << x << ", y=" << y << "\n";
}

// ========== 3. 内存管理复习 ==========

void memory_demo() {
    std::cout << "\n--- 内存管理对比 ---\n";
    
    // malloc/free（C风格）
    int* p1 = (int*)malloc(sizeof(int));
    *p1 = 100;
    std::cout << "malloc: *p1 = " << *p1 << "\n";
    free(p1);
    
    // new/delete（C++风格）
    int* p2 = new int(200);
    std::cout << "new: *p2 = " << *p2 << "\n";
    delete p2;
    
    // new[] / delete[]
    int* arr = new int[5];
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }
    std::cout << "new[]: arr = ";
    printArray(arr, 5);
    delete[] arr;
}

// ========== 4. RAII复习（不涉及拷贝/移动） ==========

// RAII管理文件
class FileRAII {
private:
    FILE* fp_;
    std::string name_;
    
public:
    // 构造函数：获取资源
    FileRAII(const std::string& name, const char* mode = "r") 
        : fp_(nullptr), name_(name) {
        fp_ = fopen(name.c_str(), mode);
        if (!fp_) {
            throw std::runtime_error("无法打开文件: " + name);
        }
        std::cout << "📂 打开文件: " << name_ << "\n";
    }
    
    // 析构函数：释放资源（唯一释放点）
    ~FileRAII() {
        if (fp_) {
            std::cout << "📂 关闭文件: " << name_ << "\n";
            fclose(fp_);
            fp_ = nullptr;
        }
    }
    
    // 禁止拷贝（防止多次释放）
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;
    
    // 获取文件指针
    FILE* get() const { return fp_; }
    
    // 写入
    void write(const std::string& text) {
        if (!fp_) throw std::runtime_error("文件未打开");
        fprintf(fp_, "%s", text.c_str());
    }
    
    // 读取一行
    std::string readLine() {
        if (!fp_) throw std::runtime_error("文件未打开");
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), fp_)) {
            return std::string(buffer);
        }
        return "";
    }
};

// RAII管理动态数组（简化版，不涉及拷贝/移动）
class IntArrayRAII {
private:
    int* data_;
    size_t size_;
    
public:
    // 构造函数：分配资源
    explicit IntArrayRAII(size_t n) : size_(n) {
        data_ = new int[n];
        for (size_t i = 0; i < n; ++i) {
            data_[i] = 0;
        }
        std::cout << "📦 分配 " << n << " 个int\n";
    }
    
    // 析构函数：释放资源
    ~IntArrayRAII() {
        if (data_) {
            std::cout << "🗑️  释放 " << size_ << " 个int\n";
            delete[] data_;
            data_ = nullptr;
        }
    }
    
    // 禁止拷贝
    IntArrayRAII(const IntArrayRAII&) = delete;
    IntArrayRAII& operator=(const IntArrayRAII&) = delete;
    
    // 访问
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
    
    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << data_[i];
            if (i < size_ - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

// ========== 5. 综合测试 ==========

void test_raii_file() {
    std::cout << "\n--- FileRAII 测试 ---\n";
    try {
        {
            FileRAII file("test_review.txt", "w+");
            file.write("第1周复习\n");
            file.write("const/引用/内存管理/RAII\n");
            // 离开作用域自动关闭
        }
        
        {
            FileRAII file("test_review.txt", "r");
            std::string line;
            while ((line = file.readLine()) != "") {
                std::cout << "读取: " << line;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ 异常: " << e.what() << "\n";
    }
}

void test_raii_array() {
    std::cout << "\n--- IntArrayRAII 测试 ---\n";
    {
        IntArrayRAII arr(5);
        for (size_t i = 0; i < arr.size(); ++i) {
            arr[i] = static_cast<int>(i * i);
        }
        arr.print();
        // 离开作用域自动释放
    }
    std::cout << "✅ 数组已自动释放\n";
}

// ========== 主函数 ==========

int main() {
    std::cout << "========================================\n";
    std::cout << "  第1周复习（已学内容）\n";
    std::cout << "  const | 引用 | 内存管理 | RAII\n";
    std::cout << "========================================\n";
    
    // const
    std::cout << "\n=== 1. const ===\n";
    const_pointer_demo();
    test_const_member();
    test_const_return();
    
    // 引用
    std::cout << "\n=== 2. 引用 ===\n";
    reference_demo();
    
    // 内存管理
    memory_demo();
    
    // RAII
    std::cout << "\n=== 3. RAII ===\n";
    test_raii_file();
    test_raii_array();
    
    std::cout << "\n========================================\n";
    std::cout << "  ✅ 第1周已学内容复习完成！\n";
    std::cout << "========================================\n";
    
    return 0;
}