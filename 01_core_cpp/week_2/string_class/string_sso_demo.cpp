// string_sso_demo.cpp
// 探测 std::string 的 SSO（短字符串优化）
// 编译：g++ -std=c++17 string_sso_demo.cpp -o string_sso_demo
// 运行：./string_sso_demo

#include <iostream>
#include <string>
#include <cstdlib>

int main() {
    std::cout << "=== std::string SSO 探秘 ===" << std::endl;
    
    // 1. 查看 string 对象的大小（通常为 32 字节，不同平台/编译器可能不同）
    std::cout << "sizeof(std::string) = " << sizeof(std::string) << " 字节" << std::endl;
    
    // 2. 取一个栈上局部变量的地址，作为“栈空间”参考
    int stack_dummy = 0;
    std::cout << "栈上局部变量地址: " << &stack_dummy << std::endl;
    
    // 3. 在堆上分配一块内存，作为“堆空间”参考
    char* heap_dummy = (char*)malloc(1);
    std::cout << "堆上动态分配地址: " << (void*)heap_dummy << std::endl;
    free(heap_dummy); // 仅用于对比地址，用完即释放
    
    std::cout << "\n--- 短字符串（长度 <= 15）---" << std::endl;
    std::string short_str = "Hello C++";
    std::cout << "内容: " << short_str << ", 长度: " << short_str.length() << std::endl;
    // 注意：c_str() 返回的地址就是 string 对象内部存储数据的起始地址
    std::cout << "数据存储地址 (c_str): " << (void*)short_str.c_str() << std::endl;
    // 如果该地址接近 stack_dummy 的地址，说明数据在栈上（SSO 生效）
    
    std::cout << "\n--- 长字符串（长度 > 15）---" << std::endl;
    std::string long_str = "This is a very long string that definitely exceeds the SSO limit!";
    std::cout << "内容: " << long_str << ", 长度: " << long_str.length() << std::endl;
    std::cout << "数据存储地址 (c_str): " << (void*)long_str.c_str() << std::endl;
    // 如果该地址接近 heap_dummy 的地址，说明数据在堆上（动态分配）
    
    // 额外对比：修改短字符串使其变长，观察地址变化
    std::cout << "\n--- 短字符串变长（触发堆分配）---" << std::endl;
    std::string dynamic_str = "Short";
    std::cout << "变长前地址: " << (void*)dynamic_str.c_str() << std::endl;
    dynamic_str += " and now this becomes a very long string!";
    std::cout << "变长后内容: " << dynamic_str << std::endl;
    std::cout << "变长后地址: " << (void*)dynamic_str.c_str() << std::endl;
    // 此时地址应该发生跳变（从栈迁移到堆），因为原栈空间不够用了
    
    return 0;
}