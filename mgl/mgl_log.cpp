#include <mgl_log.h>

#include <mgmt_once.h>

#include <stdexcept>
#include <iostream>
#include <fstream>

namespace MG {

namespace {

    const int LOGGER_THREADS = 2;

} // anon namespace

bool                                     mgl_Logger::d_configured = false;
std::unique_ptr<mgl_Logger>              mgl_Logger::d_self       = nullptr;
std::unique_ptr<mgl_LoggerConfiguration> mgl_Logger::d_config     = nullptr;

void mgl_Logger::Configure(mgl_LoggerConfiguration * configPtr)
{
    mgl_Logger::d_config.reset(configPtr);
    mgl_Logger::d_configured = true;
}

mgl_Logger * mgl_Logger::Instance()
{
    MGMT_ONCE_DO {
        d_self.reset(new mgl_Logger());
    }

    return d_self.get();

}

mgl_Logger::mgl_Logger() :
    d_threadpool(LOGGER_THREADS)
{
    if (!d_configured) {
        throw std::runtime_error("Logger is not configured!");
    }

    d_out.open(d_config->logfile(),
               std::ofstream::out | std::ofstream::app);

    if (!d_out.is_open()) {
        std::cerr << "Failed to open logfile: "
                  << d_config->logfile() << "\n";
        throw std::runtime_error("Failed to create logfile.");
    }

    if (!d_threadpool.start()) {
        throw std::runtime_error("Failed to start threadpool.");
    }
}

mgl_Logger::~mgl_Logger()
{
    d_threadpool.drain();
    d_out.close();
}

void mgl_Logger::log(const mgl_LogRecord & rec)
{
    if (!d_configured) {
        std::cerr << "Logger is not configured!\n";
        return;
    }

    // should this check be part of lambda?
    if (rec.severity() <= d_config->threshold()) {
        d_threadpool.enqueue([&, rec]() {
                std::lock_guard<std::mutex> guard(d_fileMutex);
                mgl_Logger::d_out << rec;
            });
    }

    if (rec.severity() <= d_config->stdoutThreshold()) {
        d_threadpool.enqueue([&, rec]() {
                std::lock_guard<std::mutex> guard(d_stdoutMutex);
                std::cout << rec;
            });
    }
}

} // namespace
