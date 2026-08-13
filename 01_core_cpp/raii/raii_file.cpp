// raii_file.cpp
// 演示：手动管理 vs RAII管理文件资源
// 编译：g++ -std=c++17 raii_file.cpp -o raii_file
// 运行：./raii_file

#include <iostream>
#include <cstdio>   // FILE, fopen, fclose
#include <stdexcept>

// ========== 方式1：手动管理（不安全） ==========
void manual_file_management() {
    std::cout << "\n=== 手动管理文件（危险）===\n";
    
    FILE* fp = fopen("test_manual.txt", "w");
    if (!fp) {
        std::cerr << "打开文件失败\n";
        return;
    }
    
    // 写入数据
    fprintf(fp, "Hello, Manual Management!\n");
    fflush(fp);
    
    // 模拟：如果在使用过程中发生异常或提前返回...
    // throw std::runtime_error("出错了！");  // 取消注释测试
    
    // ⚠️ 如果上面的异常被触发，这行不会执行 → 资源泄漏！
    fclose(fp);
    std::cout << "文件已手动关闭\n";
}

// ========== 方式2：RAII管理（安全） ==========
class FileRAII {
private:
    FILE* fp_;
    std::string filename_;
    
public:
    // 构造函数：获取资源
    explicit FileRAII(const std::string& filename, const char* mode = "r") 
        : fp_(nullptr), filename_(filename) {
        std::cout << "📂 打开文件: " << filename_ << "\n";
        fp_ = fopen(filename_.c_str(), mode);
        if (!fp_) {
            throw std::runtime_error("无法打开文件: " + filename_);
        }
    }
    
    // 析构函数：释放资源
    ~FileRAII() {
        if (fp_) {
            std::cout << "📂 自动关闭文件: " << filename_ << "\n";
            fclose(fp_);
            fp_ = nullptr;
        }
    }
    
    // 禁止拷贝（独占资源）
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;
    
    // 允许移动
    FileRAII(FileRAII&& other) noexcept 
        : fp_(other.fp_), filename_(std::move(other.filename_)) {
        other.fp_ = nullptr;
    }
    
    FileRAII& operator=(FileRAII&& other) noexcept {
        if (this != &other) {
            if (fp_) fclose(fp_);
            fp_ = other.fp_;
            filename_ = std::move(other.filename_);
            other.fp_ = nullptr;
        }
        return *this;
    }
    
    // 获取原生FILE*（兼容C函数）
    FILE* get() const { return fp_; }
    
    // 检查文件是否有效
    bool is_open() const { return fp_ != nullptr; }
    
    // 写入数据（便捷方法）
    void write(const std::string& text) {
        if (!fp_) throw std::runtime_error("文件未打开");
        fprintf(fp_, "%s", text.c_str());
        fflush(fp_);
    }
    
    // 读取一行
    std::string read_line() {
        if (!fp_) throw std::runtime_error("文件未打开");
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), fp_)) {
            return std::string(buffer);
        }
        return "";
    }
};

void raii_file_demo() {
    std::cout << "\n=== RAII管理文件（安全）===\n";
    
    try {
        // 创建RAII对象，自动打开文件
        FileRAII file("test_raii.txt", "w+");
        file.write("Hello, RAII Management!\n");
        file.write("This is line 2.\n");
        
        // 重新定位到文件开头
        fseek(file.get(), 0, SEEK_SET);
        
        // 读取并打印
        std::string line = file.read_line();
        std::cout << "读取: " << line;
        
        // 模拟异常
        // throw std::runtime_error("模拟异常！");
        
        std::cout << "文件操作成功完成\n";
        
        // ✅ 离开作用域时，file的析构函数自动调用fclose()
        // 即使发生异常，析构函数也会执行（栈展开）
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 异常: " << e.what() << "\n";
        // 即使这里捕获异常，FileRAII的析构函数已经释放了资源
    }
    
    std::cout << "✅ RAII对象离开作用域后，资源已自动释放\n";
}

int main() {
    std::cout << "========== RAII 文件管理演示 ==========\n";
    
    // 演示手动管理（可能泄漏）
    manual_file_management();
    
    // 演示RAII管理（安全）
    raii_file_demo();
    
    // 验证：文件是否真正被关闭？
    // 可以通过 lsof 命令查看文件是否被占用
    std::cout << "\n✅ 所有资源已正确释放\n";
    
    return 0;
}