#include <libloaderapi.h>

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
    HMODULE dll_handler_;
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

    dll_handler_ = ::LoadLibraryA(dll_path.c_str());

    return true;
}

void DynamicLoadLibrary::Impl::unload()
{
    if (dll_handler_ != nullptr) {
        ::FreeLibrary(dll_handler_);
        dll_handler_ = nullptr;
    }
}

DynamicLoadLibrary::Impl::Symbol
DynamicLoadLibrary::Impl::findSymbol(const std::string &symbol_name)
{
    if (nullptr == dll_handler_) {
        return nullptr;
    }

    return (Symbol)::GetProcAddress(dll_handler_, symbol_name.c_str());
}

} // namespace brickred::moment::base
