#include <brickred/moment/display/display_driver_x11.h>

#include <X11/Xlib.h>

#include <brickred/moment/base/dynamic_load_library.h>
#include <brickred/moment/base/internal_logger.h>

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
    finalize();
}

bool DisplayDriverX11::Impl::init()
{
    if (x_lib_dll_.load("libX11.so.6") == false) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "failed to load libX11.so.6");
        return false;
    }

    fn_x_open_display_ =
        (FN_XOpenDisplay)x_lib_dll_.findSymbol("XOpenDisplay");
    if (nullptr == fn_x_open_display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "failed to find symbol XOpenDisplay in X lib");
        return false;
    }

    return true;
}

void DisplayDriverX11::Impl::finalize()
{
    fn_x_open_display_ = nullptr;

    x_lib_dll_.unload();
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
