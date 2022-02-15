#include <brickred/moment/base/dynamic_load_library.h>

#if defined(BRICKRED_MOMENT_BUILD_PLATFORM_LINUX)
#include "dynamic_load_library_posix.cc"
#endif

namespace brickred::moment::base {

DynamicLoadLibrary::DynamicLoadLibrary() :
    pimpl_(new Impl())
{
}

DynamicLoadLibrary::~DynamicLoadLibrary()
{
}

bool DynamicLoadLibrary::load(const std::string &dll_path)
{
    return pimpl_->load(dll_path);
}

void DynamicLoadLibrary::unload()
{
    pimpl_->unload();
}

DynamicLoadLibrary::Symbol
DynamicLoadLibrary::findSymbol(const std::string &symbol_name)
{
    return pimpl_->findSymbol(symbol_name);
}

} // namespace brickred::moment::base
