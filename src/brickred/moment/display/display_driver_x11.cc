#include "brickred/moment/display/display_driver_x11.h"

namespace brickred::moment::display {

class DisplayDriverX11::Impl {
public:
    Impl();
    ~Impl();
};

DisplayDriverX11::Impl::Impl()
{
}

DisplayDriverX11::Impl::~Impl()
{
}

///////////////////////////////////////////////////////////////////////////////
DisplayDriverX11::DisplayDriverX11() :
    pimpl_(new Impl())
{
}

DisplayDriverX11::~DisplayDriverX11()
{
}

} // namespace brickred::moment::display
