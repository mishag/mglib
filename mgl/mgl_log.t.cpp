#include <mgl_log.h>

#include <vector>
#include <thread>
#include <sstream>

void doStuff()
{
    MGL_LOG_INFO << "Hello there!"
                 << MGL_LOG_END;
}

int main(int argc, char * argv[])
{
    MG::mgl_LoggerConfiguration * configPtr =
        new MG::mgl_LoggerConfiguration("./test.log",
                                        MG::mgl_Severity::INFO,
                                        MG::mgl_Severity::WARN);

    MG::mgl_Logger::Configure(configPtr);

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread(doStuff));
    }

    for (auto & th : threads) {
        th.join();
    }

    MGL_LOG_WARN << "Exitting main."
                 << MGL_LOG_END;

    return 0;
}
