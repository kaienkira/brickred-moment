#include <dlfcn.h>

namespace brickred::moment::base {

class DynamicLoadLibrary::Impl {
public:
    using Symbol = DynamicLoadLibrary::Symbol;

    Impl();
    ~Impl();

    bool load(const std::string &dll_path);
    void unload();
    Symbol findSymbol(const std::string &symbol_name);

private:
    void *dll_handler_;
};

//////////////////////////////////////////////////////////////////////////////
DynamicLoadLibrary::Impl::Impl() :
    dll_handler_(nullptr)
{
}

DynamicLoadLibrary::Impl::~Impl()
{
    unload();
}

bool DynamicLoadLibrary::Impl::load(const std::string &dll_path)
{
    if (dll_handler_ != nullptr) {
        return false;
    }

    dll_handler_ = ::dlopen(dll_path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (nullptr == dll_handler_) {
        return false;
    }

    return true;
}

void DynamicLoadLibrary::Impl::unload()
{
    if (dll_handler_ != nullptr) {
        ::dlclose(dll_handler_);
        dll_handler_ = nullptr;
    }
}

DynamicLoadLibrary::Impl::Symbol
DynamicLoadLibrary::Impl::findSymbol(const std::string &symbol_name)
{
    if (nullptr == dll_handler_) {
        return nullptr;
    }

    return ::dlsym(dll_handler_, symbol_name.c_str());
}

} // namespace brickred::moment::base
