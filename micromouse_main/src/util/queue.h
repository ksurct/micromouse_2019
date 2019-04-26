#ifndef QUEUE_H
#define QUEUE_H

template <class T, int maxSize>
class queue {
    public:
        queue(T nil);
        int size() const;
        bool empty() const;
        bool full() const;
        bool push(const T val);
        const T pop();
    
    private:
        T nil;
        T arr[maxSize];
        int front;
        int count;
};

// All needed queues:
#ifndef ARDUINO
template class queue<int, 10>;
#endif

#endif
