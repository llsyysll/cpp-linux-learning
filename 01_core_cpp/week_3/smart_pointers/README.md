# 智能指针

# 裸指针问题 + AutoPtr

## 1. 裸指针的三大致命问题

| 问题 | 描述 | 示例 |
|------|------|------|
| **内存泄漏** | `new` 后忘记 `delete`，资源永不释放 | `Resource* p = new Resource(1);` 没有 `delete` |
| **悬垂指针** | `delete` 后继续使用指针，访问已释放内存 | `delete p; p->use();` |
| **重复释放** | 同一块内存被 `delete` 两次，程序崩溃 | `delete p; delete p;` |

```cpp
// 问题1：内存泄漏
void test_leak() {
    Resource* p = new Resource(1);
    p->use();
    // 忘记 delete → 资源永不释放
}

// 问题2：悬垂指针
void test_dangling() {
    Resource* p = new Resource(2);
    delete p;
    p->use();  // 未定义行为：访问已释放内存
}

// 问题3：重复释放
void test_double_free() {
    Resource* p = new Resource(3);
    delete p;
    delete p;  // 崩溃：double free detected
}
```

## 结论

| 对比维度 | **裸指针 (Raw Pointer)** | **AutoPtr (拷贝转移)** | **明天要写的 unique_ptr** |
| :--- | :--- | :--- | :--- |
| **资源管理** | 手动 new/delete，极易遗漏 | 析构自动释放（RAII） | 析构自动释放（RAII） |
| **拷贝语义** | 浅拷贝（多个指针指向同一块内存） | **转移所有权**（拷贝后原指针变空） | **禁止拷贝**（编译期拦截） |
| **移动语义** | 不支持 | 不支持（拷贝即转移，语义混乱） | **支持移动**（std::move 显式转移） |
| **安全性** | 极低（泄漏/悬垂/重复释放） | 中等（但误用拷贝会导致空指针崩溃） | **高**（独占所有权，清晰无歧义） |
| **标准库状态** | 无处不在 | C++98 引入，C++17 已废弃 | C++11 引入，现代 C++ 首选 |

