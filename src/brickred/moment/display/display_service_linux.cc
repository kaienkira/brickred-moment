namespace brickred::moment::display {

class DisplayService::Impl {
public:
    Impl();
    ~Impl();

    bool init();
    void finalize();

    bool connect();

private:
};

///////////////////////////////////////////////////////////////////////////////
DisplayService::Impl::Impl()
{
}

DisplayService::Impl::~Impl()
{
}

bool DisplayService::Impl::init()
{
    return true;
}

void DisplayService::Impl::finalize()
{
}

bool DisplayService::Impl::connect()
{
    return true;
}

} // namespace brickred::moment::display
