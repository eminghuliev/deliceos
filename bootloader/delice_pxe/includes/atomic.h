#ifndef _ATOMIC_H
#define _ATOMIC_H
#include <atomic>
template <typename T> struct Guard;

template <class T>
class Lock {
    public:
        Lock(T value): val(value) {}
        std::atomic_uint acquire{0};
        std::atomic_uint release {0};
        Guard<T> lock();
        T val;
};

template <typename T>
struct Guard {
    Lock<T>* cell;
    T& operator*(){
        return cell->val;
    }
    T* operator->(){
        return &(cell->val);
    }
    Guard(Guard &other) = delete;
    Guard(Guard &&other) = default;
    ~Guard();
};

template <typename T>
Guard<T>::~Guard(){
    cell->release.fetch_add(1, std::memory_order_seq_cst);
}

template <class T>
Guard<T> Lock<T>::lock(){
    // It does "lock inc" instr to atomically increment 
    // counter that variable by 1.
    auto ticket = acquire.fetch_add(1, std::memory_order_seq_cst);
    while(release.load(std::memory_order_seq_cst) != ticket){
        __builtin_ia32_pause();
    }
    return Guard<T> {cell: this};
}
#endif
