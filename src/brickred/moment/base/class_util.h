#ifndef BRICKRED_MOMENT_BASE_CLASS_UTIL_H
#define BRICKRED_MOMENT_BASE_CLASS_UTIL_H

#define BRICKRED_MOMENT_NONCOPYABLE(_T) \
    _T(const _T &) = delete;            \
    _T(_T &&) = delete;                 \
    _T &operator=(const _T &) = delete; \
    _T &operator=(_T &&) = delete;      \

#define BRICKRED_MOMENT_SINGLETON(_T)   \
public:                                 \
    static _T *getInstance()            \
    {                                   \
        static _T obj;                  \
        return &obj;                    \
    }                                   \
                                        \
private:                                \
    _T();                               \
    ~_T();                              \
    _T(const _T &) = delete;            \
    _T(_T &&) = delete;                 \
    _T &operator=(const _T &) = delete; \
    _T &operator=(_T &&) = delete;      \

#endif
