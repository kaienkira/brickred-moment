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
    return true;
}

void DynamicLoadLibrary::Impl::unload()
{
}

DynamicLoadLibrary::Impl::Symbol
DynamicLoadLibrary::Impl::findSymbol(const std::string &symbol_name)
{
    return nullptr;
}

} // namespace brickred::moment::base
