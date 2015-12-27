#ifndef INCLUDED_MGMT_FIXEDQUEUE_H
#define INCLUDED_MGMT_FIXEDQUEUE_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace MG   {
namespace mgmt {

    // =================
    // class FixedQueue
    // =================

template <class T>
class FixedQueue {

private:
    typedef std::condition_variable      Condition;
    typedef std::mutex                   Mutex;
    typedef std::atomic<bool>            AtomicBool;
    typedef std::unique_lock<std::mutex> LockGuard;

    std::vector<T>     d_queue;
    size_t             d_capacity;
    AtomicBool         d_isActive;
    mutable Mutex      d_mutex;
    mutable Condition  d_condNotEmpty;
    mutable Condition  d_condNotFull;

    FixedQueue(const FixedQueue&);
    FixedQueue& operator=(const FixedQueue&);

public:
    explicit FixedQueue(size_t numElements);
    ~FixedQueue();

    void     enqueue(const T& elem);
    void     dequeue();
    const T& front() const;

    size_t capacity() const;
    size_t size()     const;
    bool   empty()    const;

}; // class mgmt_FixedQueue

    // =========================
    // FixedQueue Implementation
    // =========================

template <class T>
FixedQueue<T>::FixedQueue(size_t numElements)
    : d_queue()
    , d_capacity(numElements)
    , d_mutex()
    , d_condNotEmpty()
    , d_condNotFull()
    , d_isActive(true)
{
}

template <class T>
FixedQueue<T>::~FixedQueue()
{
    d_isActive = false;
    d_condNotEmpty.notify_all();
    d_condNotFull.notify_all();
}

template <class T>
void FixedQueue<T>::enqueue(const T& elem)
{
    LockGuard guard(d_mutex);

    while (d_isActive && d_queue.size() >= d_capacity) {
        d_condNotFull.wait(guard);
    }

    if (d_isActive) {
        d_queue.push_back(elem);
        d_condNotEmpty.notify_one();
    }
}

template <class T>
void FixedQueue<T>::dequeue()
{
    LockGuard guard(d_mutex);

    while (d_isActive && d_queue.empty()) {
        d_condNotEmpty().wait(guard);
    }

    if (d_isActive) {
        d_queue.pop_front();
        d_condNotFull.notify_one();
    }
}

template <class T>
const T& FixedQueue<T>::front() const
{
    LockGuard guard(d_mutex);
    return d_queue.front();
}

template <class T>
bool FixedQueue<T>::empty() const
{
    LockGuard guard(d_mutex);
    return d_queue.empty();
}

template <class T>
size_t FixedQueue<T>::size() const
{
    LockGuard guard(d_mutex);
    return d_queue.size();
}

template <class T>
size_t FixedQueue<T>::capacity() const
{
    return d_capacity;
}

} // namespace mgmt
} // namespace MG

#endif
