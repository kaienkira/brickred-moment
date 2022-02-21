#include <brickred/moment/base/internal_logger.h>

#include <cstdio>

namespace brickred::moment::base {

BRICKRED_MOMENT_PRECREATED_SINGLETON_IMPL(InternalLogger)

static void defaultLogFunc(int level, const char *format, va_list args) {
    if (level < InternalLogger::LogLevel::MIN ||
        level >= InternalLogger::LogLevel::MAX) {
        return;
    }

    static const char *log_level_string[] = {
        "DEBUG",
        "WARNING",
        "ERROR",
    };

    ::fprintf(stderr, "[%s] ", log_level_string[level]);
    ::vfprintf(stderr, format, args);
    ::fprintf(stderr, "\n");
}

///////////////////////////////////////////////////////////////////////////////
InternalLogger::InternalLogger() :
    log_func_(defaultLogFunc)
{
}

InternalLogger::~InternalLogger()
{
}

void InternalLogger::setLogFunc(LogFunc log_func)
{
    log_func_ = log_func;
}

void InternalLogger::log(int level, const char *format, ...)
{
    if (nullptr == log_func_) {
        return;
    }
}

} // namespace brickred::moment::base
