#ifndef BRICKRED_MOMENT_BASE_INTERNAL_LOGGER_H
#define BRICKRED_MOMENT_BASE_INTERNAL_LOGGER_H

#include <cstdarg>

#include <brickred/moment/base/class_util.h>

namespace brickred::moment::base {

class InternalLogger {
public:
    struct LogLevel {
        enum type {
            L_MIN = 0,

            L_DEBUG = 0,
            L_WARNING,
            L_ERROR,

            L_MAX
        };
    };

    using LogFunc = void (*)(int level, const char *format, va_list args);

    void setLogFunc(LogFunc log_func);
    void log(int level, const char *format, ...);

private:
    BRICKRED_MOMENT_PRECREATED_SINGLETON(InternalLogger)

    LogFunc log_func_;
};

#define BRICKRED_MOMENT_INTERNAL_LOG_DEBUG(_format, ...)           \
    brickred::moment::base::InternalLogger::getInstance()->log(    \
        brickred::moment::base::InternalLogger::LogLevel::L_DEBUG, \
        _format, ##__VA_ARGS__)
#define BRICKRED_MOMENT_INTERNAL_LOG_WARNING(_format, ...)           \
    brickred::moment::base::InternalLogger::getInstance()->log(      \
        brickred::moment::base::InternalLogger::LogLevel::L_WARNING, \
        _format, ##__VA_ARGS__)
#define BRICKRED_MOMENT_INTERNAL_LOG_ERROR(_format, ...)           \
    brickred::moment::base::InternalLogger::getInstance()->log(    \
        brickred::moment::base::InternalLogger::LogLevel::L_ERROR, \
        _format, ##__VA_ARGS__)

} // namespace brickred::moment::base

#endif
