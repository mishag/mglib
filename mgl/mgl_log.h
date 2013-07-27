#ifndef INCLUDED_MGL_LOG_H
#define INCLUDED_MGL_LOG_H

#include <mgl_severity.h>
#include <mgl_logrecord.h>
#include <mgl_loggerconfiguration.h>

#include <mgmt_threadpool.h>

#include <fstream>
#include <mutex>
#include <memory>

#define MGL_LOG(SEVERITY) {                      \
    MG::mgl_LogRecord __rEc;                     \
    __rEc.lineNumber() = __LINE__;               \
    std::ostringstream threadId;                 \
    threadId << std::this_thread::get_id();      \
    __rEc.threadId() = threadId.str();           \
    __rEc.timestamp() = time(0);                 \
    __rEc.fileName() = __FILE__;                 \
    __rEc.severity() = SEVERITY;                 \
    std::ostringstream __mSg;                    \
    __mSg

#define MGL_LOG_FATAL MGL_LOG(MG::mgl_Severity::FATAL)
#define MGL_LOG_ERROR MGL_LOG(MG::mgl_Severity::ERROR)
#define MGL_LOG_WARN MGL_LOG(MG::mgl_Severity::WARN)
#define MGL_LOG_INFO MGL_LOG(MG::mgl_Severity::INFO)
#define MGL_LOG_DEBUG MGL_LOG(MG::mgl_Severity::DEBUG)
#define MGL_LOG_TRACE MGL_LOG(MG::mgl_Severity::TRACE)

#define MGL_LOG_END                              \
    "";                                          \
    __rEc.message() = __mSg.str();               \
    MG::mgl_Logger::Instance()->log(__rEc); }


namespace MG {

class mgl_Logger {
public:
    static mgl_Logger * Instance();
    static void Configure(mgl_LoggerConfiguration * configPtr);
    void log(const mgl_LogRecord & rec);
    ~mgl_Logger();
private:
    mgl_Logger();
private:
    static std::unique_ptr<mgl_Logger>              d_self;
    static std::unique_ptr<mgl_LoggerConfiguration> d_config;
    static bool                                     d_configured;
    std::ofstream                                   d_out;
    mgmt_ThreadPool                                 d_threadpool;
    std::mutex                                      d_fileMutex;
    std::mutex                                      d_stdoutMutex;

};

} // namespace

#endif
