#ifndef INCLUDED_MGL_LOGGERCONFIGURATION_H
#define INCLUDED_MGL_LOGGERCONFIGURATION_H

#include <mgl_severity.h>

#include <string>

namespace MG {

class mgl_LoggerConfiguration {
public:
    mgl_LoggerConfiguration(const std::string & logfile,
                            mgl_Severity::Value threshold,
                            mgl_Severity::Value stdoutThreshold) :
        d_logfile(logfile),
        d_threshold(threshold),
        d_stdoutThreshold(stdoutThreshold)
    {
    }

    const std::string & logfile() const
    {
        return d_logfile;
    }

    const mgl_Severity::Value & threshold() const
    {
        return d_threshold;
    }

    const mgl_Severity::Value & stdoutThreshold() const
    {
        return d_stdoutThreshold;
    }


private:
    std::string         d_logfile;
    mgl_Severity::Value d_threshold;
    mgl_Severity::Value d_stdoutThreshold;
};

} // namespace

#endif
