// raii_lock.cpp
// RAII管理pthread_mutex_t：构造加锁，析构解锁
// 编译：g++ -std=c++17 -pthread raii_lock.cpp -o raii_lock
// 运行：./raii_lock

#include <iostream>
#include <pthread.h>
#include <unistd.h>

// ============================================
// RAII锁封装（基于pthread_mutex_t）
// ============================================
class MutexLock {
public:
    explicit MutexLock(pthread_mutex_t* mutex) : mutex_(mutex) {
        pthread_mutex_lock(mutex_);
        std::cout << "[Lock] 已加锁" << std::endl;
    }

    ~MutexLock() {
        pthread_mutex_unlock(mutex_);
        std::cout << "[Unlock] 已解锁" << std::endl;
    }

    // 禁止拷贝和赋值（确保资源唯一所有权）
    MutexLock(const MutexLock&) = delete;
    MutexLock& operator=(const MutexLock&) = delete;

private:
    pthread_mutex_t* mutex_;
};

// ============================================
// 测试：共享计数器
// ============================================
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
int g_counter = 0;

void* worker(void* arg) {
    for (int i = 0; i < 10000; ++i) {
        MutexLock lock(&g_mutex);   // RAII：进入作用域加锁
        ++g_counter;               // 临界区操作
    }                               // 离开作用域自动解锁
    return nullptr;
}

int main() {
    const int THREAD_COUNT = 4;
    pthread_t threads[THREAD_COUNT];

    std::cout << "=== RAII互斥锁测试 ===" << std::endl;
    std::cout << "启动 " << THREAD_COUNT << " 个线程，每个累加10000次" << std::endl;

    // 创建线程
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_create(&threads[i], nullptr, worker, nullptr);
    }

    // 等待线程结束
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "最终计数器值: " << g_counter << std::endl;
    std::cout << "预期值: " << THREAD_COUNT * 10000 << std::endl;

    if (g_counter == THREAD_COUNT * 10000) {
        std::cout << "✅ RAII锁工作正常！" << std::endl;
    } else {
        std::cout << "❌ 存在数据竞争！" << std::endl;
    }

    pthread_mutex_destroy(&g_mutex);
    return 0;
}