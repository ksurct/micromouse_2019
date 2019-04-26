#ifndef QUEUE_H
#define QUEUE_H

template <typename T, int maxSize>
class queue {
    public:
        queue(T _nil) : nil(_nil), front(0), count(0) {
        }

        int size() const {
            return count;
        }

        bool empty() const {
            return count == 0;
        }

        bool full() const {
            return count == maxSize;
        }

        bool push(const T val) {
            if (count == maxSize) {
                return false;
            }
            arr[(front + count++) % maxSize] = val;
            return true;
        }

        const T pop() {
            if (count == 0) {
                return nil;
            }
            T val = arr[front];
            front = (front + 1) % maxSize;
            --count;
            return val;
        }
    
    private:
        T nil;
        T arr[maxSize];
        int front;
        int count;
};

template <typename T> void swap (T& a, T& b) {
    T c(a);
    a = b;
    b = c;
}

#endif
