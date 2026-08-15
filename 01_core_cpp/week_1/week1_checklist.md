# 第1周 自查清单（2026.8.10-8.14）

## ✅ 已学内容确认

### const（8.10）
- [√] const修饰变量：`const int MAX = 100;`
- [ ] const修饰指针：`const int* p` / `int* const p` / `const int* const p`
- [ ] const修饰成员函数：`int get() const;`
- [ ] const修饰返回值：`const int& getRef();`
- [ ] mutable成员变量：`const函数中可修改`

### 引用与指针（8.11）
- [ ] 引用的定义和使用：`int& ref = a;`
- [ ] 引用作为函数参数（避免拷贝）
- [ ] 引用作为函数返回值（必须确保对象存在）
- [ ] 引用 vs 指针的区别（3个以上）

### 内存管理（8.12）
- [ ] new/delete 和 malloc/free 的区别
- [ ] new[] / delete[] 配对使用
- [ ] 栈内存 vs 堆内存
- [ ] 内存泄漏的概念

### RAII（8.13-8.14）
- [ ] RAII的核心思想：资源获取即初始化
- [ ] 构造函数获取资源，析构函数释放资源
- [ ] 为什么RAII是异常安全的
- [ ] 禁止拷贝（= delete）
- [ ] 能手写FileRAII类
- [ ] 能手写MutexRAII类

## 🔍 知识小测验

### 1. 下面哪个是正确的const指针声明？
A. const int* p = &a;   // p指向const int
B. int const* p = &a;   // 同A
C. int* const p = &a;   // p本身是const
D. 以上都对
答案：D

### 2. 引用和指针的区别？
引用：必须初始化、不能重新绑定、语法像普通变量
指针：可以为nullptr、可以改变指向、需要*解引用

### 3. 为什么RAII能保证异常安全？
```text
因为C++的栈展开机制：抛出异常时，所有栈上对象的析构函数
都会被调用，RAII对象在析构时释放资源。
```
### 4. 下面代码有什么问题？
```cpp
FILE* fp = fopen("test.txt", "r");
if (some_condition) {
    return;  // ⚠️ 忘记fclose()
}
fclose(fp);
```
答案：提前return导致资源泄漏，应该用RAII管理