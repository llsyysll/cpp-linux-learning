# 2026-08-12 堆内存管理

## 掌握内容
- [x] malloc/free vs new/delete 区别
- [x] new 调用构造，delete 调用析构
- [x] new[] 必须配 delete[]
- [x] placement new：分离分配和构造
- [x] 内存泄漏的危害

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