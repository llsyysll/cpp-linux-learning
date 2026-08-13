# 2026-08-12 堆内存管理

## 掌握内容
- [√] malloc/free vs new/delete 区别
- [√] new 调用构造，delete 调用析构
- [√] new[] 必须配 delete[]
- [√] placement new：分离分配和构造
- [√] 内存泄漏的危害

## 核心区别
|   特性       | malloc/free  | new/delete|
|------        |------------- |----------|
| 调用构造/析构 | ❌           | ✅      |
| 返回类型      | void*         | 类型安全 |
| 失败时        | 返回 NULL     | 抛出异常 |
| 数组分配      | 手动算大小    | 自动处理 |

## 易错点
1. new/delete 配对，new[]/delete[] 配对
2. malloc 返回 void*，需要强转
3. 忘记 delete 导致内存泄漏
## 注：
四种内存管理方式
// 1. 栈对象（最简单）
MyClass obj(10, 20);  // 自动构造，自动析构

// 2. 普通 new（标准方式）
MyClass* p1 = new MyClass(10, 20);
delete p1;  // 自动析构 + 释放

// 3. malloc + placement new（手动方式）
void* mem = malloc(sizeof(MyClass));
MyClass* p2 = new (mem) MyClass(10, 20);
p2->~MyClass();
free(mem);

// 4. 智能指针（现代方式）
auto p3 = std::make_unique<MyClass>(10, 20);
// 自动管理
