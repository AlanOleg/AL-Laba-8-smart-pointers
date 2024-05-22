#include <iostream>
#include <utility>
using namespace std;

template<class T>
class MyShared {
    T* p = nullptr;
    int* count = nullptr;
    void deleter() {
        if (*count > 0) {
            --(*count);
        }
        if (*count == 0) {
            delete p;
            delete count;
        }
    }
public:
    MyShared(T* p) {
        this->p = p;
        this->count = new int(1);
    };
    MyShared(const MyShared& T2){
        this->p = T2.p;
        this->count = T2.count;
        ++(*count);
    };
    MyShared(MyShared&& T2) noexcept : p(T2.p), count(T2.count) {
        T2.p = nullptr;
        T2.count = nullptr;
    };
    ~MyShared() {
        deleter();
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
    T& operator=(const T& T2) {
        if (this != &T2) {
            deleter();
            p = T2.p;
            count = T2.count;
            *count++;
        }
        return *this;
    }
    T& operator=(T&& T2) noexcept {
        if (this != &T2) {
            deleter();
            p = T2.p;
            count = T2.count;
            T2.p = nullptr;
            T2.count = nullptr;
        }
        return *this;
    }
};

template<typename T, typename... Args>
MyShared<T> Make_MyShared(Args&&... args) {
    return MyShared<T>(new T(forward<Args>(args)...));
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
    MyShared<MyPoint> shared_pointer1 = Make_MyShared<MyPoint>(2, 3);
    shared_pointer1->print();
    cout << shared_pointer1.get() << endl;

    MyShared<MyPoint> shared_pointer2 = shared_pointer1;
    shared_pointer2->print();
    cout << shared_pointer2.get() << endl;

    if (shared_pointer1.get() == shared_pointer2.get()) {
        cout << "shared_pointer1 and shared_pointer2 link to the same thing" << endl;
    }

    MyShared<MyPoint> shared_pointer3 = move(shared_pointer1);
    shared_pointer3->print();
    cout << shared_pointer3.get() << endl;

    if (shared_pointer1.get() == nullptr) {
        cout << "shared_pointer1 is gone" << endl;
    }
}
