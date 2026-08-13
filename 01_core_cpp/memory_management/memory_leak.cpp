#include <iostream>
using namespace std;

void leak_memory() {
    int* p = new int(42);
    // 忘记 delete p;
}

int main() {
    for (int i = 0; i < 10; i++) {
        leak_memory();
    }
    cout << "泄漏了 10 次 int 内存，程序退出时由操作系统回收" << endl;
    cout << "但长时间运行的程序（如服务器）不能这样写" << endl;
    return 0;
}