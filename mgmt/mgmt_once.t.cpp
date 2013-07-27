#include <mgmt_once.h>

#include <iostream>
#include <thread>
#include <vector>

void doWork()
{
    MGMT_ONCE_DO {

        std::cout << "Thread: " << std::this_thread::get_id()
                  << " is doing work.\n";

        std::chrono::seconds secs(2);
        std::this_thread::sleep_for(secs);

        std::cout << "Thread: " << std::this_thread::get_id()
                  << " is done.\n";
    }

    std::cout << "Thread: " << std::this_thread::get_id()
              << " is through.\n";
}

int main(int argc, char * argv[])
{

    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread(doWork));
    }

    for (auto & th : threads) {
        th.join();
    }


    MGMT_ONCE_DO {
        std::cout << "Here.\n";
    }

    MGMT_ONCE_DO {
        std::cout << "There.\n";
    }

    doWork();


}
