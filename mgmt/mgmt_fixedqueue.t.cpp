#include <mgmt_fixedqueue.h>

#include <iostream>
#include <thread>

typedef MG::mgmt::FixedQueue<int> Queue;

void waitForWorkers(std::vector<std::thread>& threads)
{
    const int numThreads = threads.size();
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }
}

int main(int argc, char * argv[])
{
    Queue queue(5);

    std::vector<std::thread> threads;

    const int numThreads = 10;
    for (int i = 0; i < numThreads; ++i) {
        std::thread([i,&queue]() {
                queue.enqueue(i);
            }).detach();
    }

    // waitForWorkers();

    std::cout << "Queue size: " << queue.size() << "\n";

    return 0;
}
