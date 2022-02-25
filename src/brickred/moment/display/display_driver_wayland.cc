#include <brickred/moment/display/display_driver_wayland.h>

namespace brickred::moment::display {

class DisplayDriverWayland::Impl {
public:
    Impl();
    ~Impl();

    bool init();
    void finalize();
    bool connect();
    void disconnect();

private:
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverWayland::Impl::Impl()
{
}

DisplayDriverWayland::Impl::~Impl()
{
}

bool DisplayDriverWayland::Impl::init()
{
    return true;
}

void DisplayDriverWayland::Impl::finalize()
{
}

bool DisplayDriverWayland::Impl::connect()
{
    return true;
}

void DisplayDriverWayland::Impl::disconnect()
{
}

///////////////////////////////////////////////////////////////////////////////
DisplayDriverWayland::DisplayDriverWayland() :
    pimpl_(new Impl())
{
}

DisplayDriverWayland::~DisplayDriverWayland()
{
}

bool DisplayDriverWayland::init()
{
    return pimpl_->init();
}

void DisplayDriverWayland::finalize()
{
    pimpl_->finalize();
}

bool DisplayDriverWayland::connect()
{
    return pimpl_->connect();
}

void DisplayDriverWayland::disconnect()
{
    pimpl_->disconnect();
}

} // namespace brickred::moment::display
