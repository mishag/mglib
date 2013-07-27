#include <mgmt_threadpool.h>

#include <iostream>

namespace MG {

namespace {
    const int DEFAULT_NUM_THREADS = 4;

} // anon namespace

int mgmt_ThreadPool::numThreads() const
{
    return d_numThreads;
}

int mgmt_ThreadPool::availableThreads() const
{
    return d_availableThreads;
}

int mgmt_ThreadPool::numTasks() const
{
    std::lock_guard<std::mutex> guard(d_mutex);
    return d_queue.size();
}

int mgmt_ThreadPool::status() const
{
    return d_state;
}

void mgmt_ThreadPool::startThread()
{
    while (d_state == STARTING ||
           d_state == ACTIVE ||
           d_state == DRAINING) {

        ++d_availableThreads;

        std::unique_lock<std::mutex> lock(d_mutex);

        while (d_queue.empty() && (d_state == STARTING ||
                                   d_state == ACTIVE)) {
            d_qNotEmpty.wait(lock);
        }

        if (d_state == STOPPING || d_state == STOPPED) {
            --d_availableThreads;
            lock.unlock();
            return;
        }

        if (d_queue.empty() && d_state == DRAINING) {
            --d_availableThreads;
            lock.unlock();
            return;
        }

        --d_availableThreads;
        Task t = d_queue.front();
        d_queue.pop_front();
        lock.unlock();

        t();
    }
}

bool mgmt_ThreadPool::enqueue(const Task & task)
{
    if (d_state != ACTIVE) {
        return false;
    }

    std::lock_guard<std::mutex> guard(d_mutex);

    d_queue.push_back(task);
    d_qNotEmpty.notify_one();

    return true;
}

bool mgmt_ThreadPool::start()
{
    if (d_state != STOPPED) {
        return false;
    }

    d_state = STARTING;

    for (int i = 0; i < d_numThreads; ++i) {
        d_threads.push_back(
               std::thread(&mgmt_ThreadPool::startThread, this));
    }

    d_state = ACTIVE;

    return true;
}

void mgmt_ThreadPool::stop()
{
    if (d_state != ACTIVE) {
        return;
    }

    d_state = STOPPING;
    d_qNotEmpty.notify_all();

    for (auto & th : d_threads) {
        th.join();
    }

    d_state = STOPPED;
}

void mgmt_ThreadPool::drain()
{
    if (d_state != ACTIVE) {
        return;
    }

    d_state = DRAINING;
    d_qNotEmpty.notify_all();

    for (auto & th : d_threads) {
        th.join();
    }

    d_state = STOPPED;
}

mgmt_ThreadPool::mgmt_ThreadPool(int numThreads) :
    d_numThreads(numThreads),
    d_availableThreads(0),
    d_state(STOPPED)
{
}

mgmt_ThreadPool::mgmt_ThreadPool():
    d_numThreads(DEFAULT_NUM_THREADS),
    d_availableThreads(0),
    d_state(STOPPED)
{
}

mgmt_ThreadPool::~mgmt_ThreadPool()
{
    stop();
}

} // namespace
