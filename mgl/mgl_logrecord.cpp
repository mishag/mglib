#include <mgl_logrecord.h>

#include <ctime>
#include <time.h>

namespace MG {

namespace {
    const int MAX_TIME_STR_LEN = 100;
} // anon namespace

std::ostream & operator<<(std::ostream & o,
                          const mgl_LogRecord & logRec)
{
    std::string severity;
    mgl_Severity::toString(severity, logRec.severity());

    char timeBuf[MAX_TIME_STR_LEN] = {0};

    struct tm timeStruct;
    localtime_r(&logRec.timestamp(), &timeStruct);
    strftime(timeBuf,
             MAX_TIME_STR_LEN,
             "%Y-%m-%d_%H:%M:%S",
             &timeStruct);

    o << timeBuf << " "
      << severity << ": "
      << logRec.fileName() << ":"
      << logRec.lineNumber() << ":"
      << logRec.threadId() << ": "
      << logRec.message() << "\n";

    return o;
}

} // namespace
