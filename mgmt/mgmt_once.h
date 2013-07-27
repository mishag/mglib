#ifndef INCLUDED_MGMT_ONCE_H
#define INCLUDED_MGMT_ONCE_H

#include <atomic>
#include <mutex>

#define CONCAT_(X, Y) X##Y

#define CONCAT(X, Y) CONCAT_(X, Y)

#define UNIQ_NAME(PREFIX) CONCAT(PREFIX, __LINE__)

#define MGMT_ONCE_DO                                                \
    static bool UNIQ_NAME(taskDone) = false;                        \
    static std::atomic_flag UNIQ_NAME(ranOnce) =                    \
        ATOMIC_FLAG_INIT;                                           \
    do {                                                            \
    } while (std::atomic_flag_test_and_set(&UNIQ_NAME(ranOnce)) &&  \
             !UNIQ_NAME(taskDone));                                 \
    for (; !UNIQ_NAME(taskDone); UNIQ_NAME(taskDone) = true)


#endif
