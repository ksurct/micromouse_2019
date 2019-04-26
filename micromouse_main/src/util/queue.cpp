#include "queue.h"

template <class T, int maxSize>
queue<T, maxSize>::queue(T _nil) : nil(_nil), front(0), count(0) {
}

template <class T, int maxSize>
int queue<T, maxSize>::size() const {
    return count;
}

template <class T, int maxSize>
bool queue<T, maxSize>::empty() const {
    return count == 0;
}

template <class T, int maxSize>
bool queue<T, maxSize>::full() const {
    return count == maxSize;
}

template <class T, int maxSize>
bool queue<T, maxSize>::push(const T val) {
    if (count == maxSize) {
        return false;
    }
    arr[(front + count++) % maxSize] = val;
    return true;
}

template <class T, int maxSize>
const T queue<T, maxSize>::pop() {
    if (count == 0) {
        return nil;
    }
    T val = arr[front];
    front = (front + 1) % maxSize;
    --count;
    return val;
}
