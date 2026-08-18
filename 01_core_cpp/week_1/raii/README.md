## RAII思想
## 日期 2026年8月13日

## 核心概念

### 什么是RAII？
**RAII** (Resource Acquisition Is Initialization) 是C++中管理资源的核心设计模式。

**核心思想**：
- 构造函数负责**获取**资源
- 析构函数负责**释放**资源
- 资源生命周期与对象生命周期绑定

### 为什么需要RAII？
1. **防止资源泄漏**：异常安全，栈展开自动释放
2. **简化代码**：无需手动管理释放
3. **提高可维护性**：资源管理逻辑集中

### RAII适用场景
| 资源类型 | 手动管理 | RAII管理 |
|---------|---------|----------|
| 堆内存 | new/delete | unique_ptr/shared_ptr |
| 文件 | fopen/fclose | fstream |
| 互斥锁 | lock/unlock | lock_guard/unique_lock |
| Socket | socket/close | 自定义RAII类 |

### 关键原则
1. **获取即初始化**：在构造函数中获取资源
2. **析构释放**：在析构函数中释放资源
3. **禁止拷贝**：通常禁用拷贝，支持移动
4. **异常安全**：析构函数不应抛出异常

## 今日代码清单
- `raii_file.cpp` - RAII管理文件资源
- `raii_mutex.cpp` - RAII管理互斥锁
- `raii_comprehensive.cpp` - 综合练习

## 核心代码模式

### 标准RAII类模板
```cpp
class ResourceRAII {
private:
    Resource* res_;
public:
    ResourceRAII() : res_(acquire_resource()) {}
    ~ResourceRAII() { release_resource(res_); }
    // 禁止拷贝
    ResourceRAII(const ResourceRAII&) = delete;
    ResourceRAII& operator=(const ResourceRAII&) = delete;
    // 支持移动
    ResourceRAII(ResourceRAII&& other) noexcept { ... }
};
```

## RAII 管理互斥锁

## 核心思想
将互斥锁的**加锁**放在构造函数，**解锁**放在析构函数。

## 为什么用 RAII 管理锁？
1. **自动释放**：离开作用域自动解锁，不会忘记
2. **异常安全**：即使抛出异常，析构函数仍会被调用
3. **代码简洁**：不需要手动 `unlock`

## 关键点
- 禁止拷贝（锁不能被复制）
- 构造时加锁，析构时解锁
- 作用域控制临界区范围（用 `{}` 限定）

## 对比手动加锁/解锁
| 方式 | 优点 | 缺点 |
|------|------|------|
| 手动 `lock/unlock` | 灵活 | 容易忘记解锁，异常不安全 |
| RAII 封装 | 自动释放，异常安全 | 需要额外封装类 |

> 这也是 C++11 标准库 `std::lock_guard` 和 `std::unique_lock` 的实现原理。