// raii_mutex.cpp
// 演示：使用RAII管理互斥锁（模拟std::lock_guard原理）
// 编译：g++ -std=c++17 -pthread raii_mutex.cpp -o raii_mutex
// 运行：./raii_mutex

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>

// ========== 自定义RAII互斥锁封装 ==========
class MutexRAII {
private:
    std::mutex& mutex_;
    bool locked_;
    
public:
    // 构造函数：自动加锁
    explicit MutexRAII(std::mutex& m) 
        : mutex_(m), locked_(true) {
        mutex_.lock();
        std::cout << "🔒 线程 " << std::this_thread::get_id() 
                  << " 获取锁\n";
    }
    
    // 析构函数：自动解锁
    ~MutexRAII() {
        if (locked_) {
            mutex_.unlock();
            std::cout << "🔓 线程 " << std::this_thread::get_id() 
                      << " 释放锁\n";
        }
    }
    
    // 禁止拷贝和移动（RAII对象不可复制）
    MutexRAII(const MutexRAII&) = delete;
    MutexRAII& operator=(const MutexRAII&) = delete;
    
    // 手动解锁（提前释放）
    void unlock() {
        if (locked_) {
            mutex_.unlock();
            locked_ = false;
        }
    }
    
    // 手动加锁（重新获取）
    void lock() {
        if (!locked_) {
            mutex_.lock();
            locked_ = true;
        }
    }
};

// ========== 对比：手动加锁解锁（容易出错） ==========
void manual_lock_counter(std::mutex& mtx, int& counter, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        mtx.lock();
        counter++;
        // ⚠️ 如果在counter++和unlock之间发生异常或提前return
        // 锁永远不会释放 → 死锁！
        // 这里模拟：如果counter达到某个值，提前返回
        if (counter > iterations / 2) {
            std::cout << "⚠️ 提前返回，忘记解锁！\n";
            // mtx.unlock(); // 忘记调用 → 死锁！
            return;
        }
        mtx.unlock();
    }
}

// ========== 安全：RAII自动管理锁 ==========
void raii_lock_counter(std::mutex& mtx, int& counter, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        MutexRAII lock(mtx);  // 构造时自动加锁
        counter++;
        
        // 即使这里发生异常或提前返回，锁也会在析构时自动释放
        if (counter > iterations / 2) {
            std::cout << "✅ 提前返回，锁自动释放\n";
            return;  // ✅ 析构函数自动调用unlock()
        }
        // 离开作用域时自动解锁
    }
}

// ========== 使用std::lock_guard（标准库） ==========
void std_lock_counter(std::mutex& mtx, int& counter, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx);  // RAII
        counter++;
        // 自动加锁/解锁
    }
}

// ========== 生产者-消费者示例（使用RAII锁） ==========
template<typename T>
class ThreadSafeQueue {
private:
    std::vector<T> queue_;
    mutable std::mutex mtx_;
    
public:
    void push(T value) {
        MutexRAII lock(mtx_);  // 自动加锁
        queue_.push_back(value);
        std::cout << "✅ 生产者推送: " << value << "\n";
    }
    
    bool pop(T& value) {
        MutexRAII lock(mtx_);  // 自动加锁
        if (queue_.empty()) {
            return false;
        }
        value = queue_.front();
        queue_.erase(queue_.begin());
        std::cout << "📦 消费者弹出: " << value << "\n";
        return true;
    }
    
    size_t size() const {
        MutexRAII lock(mtx_);  // const函数也可以加锁
        return queue_.size();
    }
};

void producer(ThreadSafeQueue<int>& q, int start, int count) {
    for (int i = 0; i < count; ++i) {
        q.push(start + i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void consumer(ThreadSafeQueue<int>& q, int count) {
    int value;
    int received = 0;
    while (received < count) {
        if (q.pop(value)) {
            received++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {
    std::cout << "========== RAII 互斥锁管理演示 ==========\n\n";
    
    // ---------- 对比实验 ----------
    std::cout << "【对比1：手动加锁 vs RAII加锁】\n";
    {
        std::mutex mtx;
        int counter = 0;
        const int ITERS = 10;
        
        // 危险版本（可能死锁）
        // std::thread t1(manual_lock_counter, std::ref(mtx), 
        //                std::ref(counter), ITERS);
        // t1.join();  // 如果提前返回，死锁！不要运行
        
        std::cout << "使用RAII版本：\n";
        std::thread t2(raii_lock_counter, std::ref(mtx), 
                       std::ref(counter), ITERS);
        t2.join();
        std::cout << "counter = " << counter << "\n";
    }
    
    // ---------- 生产者-消费者 ----------
    std::cout << "\n【生产者-消费者（使用RAII锁）】\n";
    {
        ThreadSafeQueue<int> queue;
        const int NUM_PRODUCERS = 3;
        const int NUM_CONSUMERS = 2;
        const int ITEMS_PER_PRODUCER = 5;
        
        std::vector<std::thread> producers;
        std::vector<std::thread> consumers;
        
        // 启动生产者
        for (int i = 0; i < NUM_PRODUCERS; ++i) {
            producers.emplace_back(producer, std::ref(queue), 
                                   i * 100, ITEMS_PER_PRODUCER);
        }
        
        // 启动消费者
        for (int i = 0; i < NUM_CONSUMERS; ++i) {
            consumers.emplace_back(consumer, std::ref(queue), 
                                   (NUM_PRODUCERS * ITEMS_PER_PRODUCER) / NUM_CONSUMERS);
        }
        
        // 等待所有线程完成
        for (auto& t : producers) t.join();
        for (auto& t : consumers) t.join();
        
        std::cout << "最终队列大小: " << queue.size() << "\n";
    }
    
    // ---------- std::lock_guard 使用 ----------
    std::cout << "\n【使用std::lock_guard】\n";
    {
        std::mutex mtx;
        int counter = 0;
        const int ITERS = 100000;
        
        auto start = std::chrono::high_resolution_clock::now();
        std::thread t1(std_lock_counter, std::ref(mtx), 
                       std::ref(counter), ITERS / 2);
        std::thread t2(std_lock_counter, std::ref(mtx), 
                       std::ref(counter), ITERS / 2);
        t1.join();
        t2.join();
        auto end = std::chrono::high_resolution_clock::now();
        
        std::cout << "counter = " << counter << "\n";
        std::cout << "耗时: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         end - start).count() 
                  << "ms\n";
    }
    
    std::cout << "\n✅ RAII锁管理演示完成\n";
    return 0;
}