/*#include <atomic>
template <typename T> struct Guard;

template <class T>
class Lock {
    public:
        Lock(T value): val(value) {}
        std::atomic_uint acquire{0};
        std::atomic_uint release {0};
        Guard<T> lock();
    private:
        T val;
};

template <typename T>
struct Guard {
    Lock<T>* cell;
    ~Guard();
};

template <typename T>
Guard<T>::~Guard(){
    cell->release.fetch_add(1, std::memory_order_seq_cst);
}

template <class T>
Guard<T> Lock<T>::lock(){
    auto ticket = acquire.fetch_add(1, std::memory_order_seq_cst);
    while(release.load(std::memory_order_seq_cst) != ticket){}
    return Guard<T> {cell: this};
}



*/
