#include <brickred/moment/display/display_driver_x11.h>

#include <X11/Xlib.h>

#include <brickred/moment/base/dynamic_load_library.h>

namespace brickred::moment::display {

using brickred::moment::base::DynamicLoadLibrary;

class DisplayDriverX11::Impl {
public:
    using FN_XOpenDisplay = Display *(*)(const char *);

    Impl();
    ~Impl();

    bool init();
    void finalize();

private:
    DynamicLoadLibrary x_lib_dll_;
    FN_XOpenDisplay fn_x_open_display_;
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverX11::Impl::Impl() :
    fn_x_open_display_(nullptr)
{
}

DisplayDriverX11::Impl::~Impl()
{
}

bool DisplayDriverX11::Impl::init()
{
    if (x_lib_dll_.load("libX11.so.6") == false) {
        return false;
    }

    return true;
}

void DisplayDriverX11::Impl::finalize()
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

bool DisplayDriverX11::init()
{
    return pimpl_->init();
}

void DisplayDriverX11::finalize()
{
    pimpl_->finalize();
}

} // namespace brickred::moment::display
