#include <brickred/moment/display/display_driver_wayland.h>

namespace brickred::moment::display {

class DisplayDriverWayland::Impl {
public:
    Impl();
    ~Impl();
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverWayland::Impl::Impl()
{
}

DisplayDriverWayland::Impl::~Impl()
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

} // namespace brickred::moment::display
