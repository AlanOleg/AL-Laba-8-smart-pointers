#include <iostream>
#include <utility>
using namespace std;

template<class T>
class MyUnique {
    T *p = nullptr;
public:
    explicit MyUnique(T* p) {
        this->p = p;
    };
    MyUnique(MyUnique&& T2) noexcept: p(T2.p) {
        T2.p = nullptr;
    }
    MyUnique(const T&) = delete;
    ~MyUnique() {
        delete p;
    };
    T* get() const {
        return p;
    };
    T& operator*() const {
        return *p;
    };
    T* operator->() const {
        return p;
    };
    T& operator=(T&& T2) noexcept {
        if (this != &T2) {
            delete p;
            p = T2.p;
            T2.p = nullptr;
        }
        return *this;
    }
    T& operator=(const T&) = delete;  
};

template<typename T, typename... Args>
MyUnique<T> Make_MyUnique(Args&&... args) {
    return MyUnique<T>(new T(forward<Args>(args)...));
}

class MyPoint {
    int x, y;
public:
    MyPoint(int x, int y) : x(x), y(y) {
        cout << "MyPoint(int x, int y): x=" << x << " y=" << y << std::endl;
    }
    void print() const {
        cout << "Point(" << x << ", " << y << ")" << std::endl;
    }
};

int main()
{
    MyUnique<MyPoint> unique_pointer = Make_MyUnique<MyPoint>(2, 3);
    unique_pointer->print();
    cout << unique_pointer.get() << endl;
    MyUnique<MyPoint> unique_pointer2 = move(unique_pointer);
    unique_pointer2->print();
}
