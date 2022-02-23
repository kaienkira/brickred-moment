#ifndef BRICKRED_MOMENT_BASE_DYNAMIC_LOAD_LIBRARY_H
#define BRICKRED_MOMENT_BASE_DYNAMIC_LOAD_LIBRARY_H

#include <memory>
#include <string>

#include <brickred/moment/base/class_util.h>

namespace brickred::moment::base {

class DynamicLoadLibrary {
public:
    using Symbol = void *;

    DynamicLoadLibrary();
    ~DynamicLoadLibrary();

    bool load(const std::string &dll_path);
    void unload();
    Symbol findSymbol(const std::string &symbol_name);

private:
    BRICKRED_MOMENT_NONCOPYABLE(DynamicLoadLibrary)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::base

#endif
