#include <iostream>
#include <cstdlib>  // for malloc/free
using namespace std;

class Test {
public:
    int x;
    Test(int v) : x(v) {
        cout << "构造函数被调用，x = " << x << endl;
    }
    ~Test() {
        cout << "析构函数被调用，x = " << x << endl;
    }
};

int main() {
    cout << "=== malloc/free ===" << endl;
    Test* p1 = (Test*)malloc(sizeof(Test));
    if (p1) {
        cout << "malloc 分配成功，但对象未构造" << endl;
        free(p1);
    }

    cout << "\n=== new/delete ===" << endl;
    Test* p2 = new Test(42);
    cout << "p2->x = " << p2->x << endl;
    delete p2;

    cout << "\n=== new[] / delete[] ===" << endl;
    Test* p3 = new Test[3]{10, 20, 30};
    delete[] p3;

    return 0;
}