#pragma once
#include <iostream>
#include <utility>  // for std::move

template <typename T>
class MyUniquePtr {
public:
    // 1. 构造 & 析构
    explicit MyUniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {
        std::cout << "MyUniquePtr: acquire " << ptr_ << std::endl;
    }

    ~MyUniquePtr() {
        std::cout << "MyUniquePtr: release " << ptr_ << std::endl;
        delete ptr_;
    }

    // 2. 禁止拷贝（这是 unique 的关键）
    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    // 3. 移动构造（转移所有权）
    MyUniquePtr(MyUniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
        std::cout << "MyUniquePtr move construct" << std::endl;
    }

    // 4. 移动赋值（释放自身原资源，接管对方资源）
    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        std::cout << "MyUniquePtr move assignment" << std::endl;
        if (this != &other) {
            delete ptr_;          // 释放当前资源
            ptr_ = other.ptr_;    // 接管对方
            other.ptr_ = nullptr; // 置空对方
        }
        return *this;
    }

    // 5. 重载运算符（模拟原生指针行为）
    T& operator*() const noexcept {
        return *ptr_;
    }

    T* operator->() const noexcept {
        return ptr_;
    }

    // 6. 辅助工具（为明天预习）
    T* get() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

private:
    T* ptr_;
};