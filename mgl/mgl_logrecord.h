#ifndef INCLUDED_MGL_LOGRECORD_H
#define INCLUDED_MGL_LOGRECORD_H

#include <mgl_severity.h>

#include <string>
#include <ostream>

namespace MG {

class mgl_LogRecord {
public:

    int & lineNumber()
    {
        return d_lineNumber;
    }

    const int & lineNumber() const
    {
        return d_lineNumber;
    }

    time_t & timestamp()
    {
        return d_timestamp;
    }

    const time_t & timestamp() const
    {
        return d_timestamp;
    }

    std::string & fileName()
    {
        return d_fileName;
    }

    const std::string & fileName() const
    {
        return d_fileName;
    }

    std::string & message()
    {
        return d_message;
    }

    const std::string & message() const
    {
        return d_message;
    }

    mgl_Severity::Value & severity()
    {
        return d_severity;
    }

    const mgl_Severity::Value & severity() const
    {
        return d_severity;
    }

    const std::string & threadId() const
    {
        return d_threadId;
    }

    std::string & threadId()
    {
        return d_threadId;
    }

    friend std::ostream & operator<<(std::ostream & o,
                                     const mgl_LogRecord & logRec);

private:
    int                 d_lineNumber;
    std::string         d_threadId;
    time_t              d_timestamp;
    std::string         d_fileName;
    std::string         d_message;
    mgl_Severity::Value d_severity;
};

}  // namespace

#endif
