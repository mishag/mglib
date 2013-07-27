#ifndef INCLUDED_MGL_SEVERITY_H
#define INCLUDED_MGL_SEVERITY_H

#include <string>

namespace MG {

struct mgl_Severity {

    typedef enum {FATAL = 0,
                  ERROR = 1,
                  WARN  = 2,
                  INFO  = 3,
                  DEBUG = 4,
                  TRACE = 5} Value;

    static void toString(std::string & str,
                         mgl_Severity::Value val)
    {
        switch(val) {
        case FATAL :
            str = "FATAL";
            break;
        case ERROR :
            str = "ERROR";
            break;
        case WARN:
            str = "WARN ";
            break;
        case INFO:
            str = "INFO ";
            break;
        case DEBUG:
            str = "DEBUG";
            break;
        case TRACE:
            str = "TRACE";
            break;
        default:
            str = "UNKNOWN_SEVERITY";
            break;
        }
    }

    static bool fromInt(mgl_Severity::Value & val,
                        int intVal)
    {
        switch (intVal) {
        case 0:
            val = FATAL;
            return true;
        case 1:
            val = ERROR;
            return true;
        case 2:
            val = WARN;
            return true;
        case 3:
            val = INFO;
            return true;
        case 4:
            val = DEBUG;
            return true;
        case 5:
            val = TRACE;
            return true;
        }

        return false;
    }
};

} // namespace

#endif
