#include <brickred/moment/display/display_driver_x11.h>

#include <X11/Xlib.h>

#include <brickred/moment/base/dynamic_load_library.h>
#include <brickred/moment/base/internal_logger.h>

namespace brickred::moment::display {

using brickred::moment::base::DynamicLoadLibrary;

class DisplayDriverX11::Impl {
public:
    using FN_XOpenDisplay = Display *(*)(const char *);
    using FN_XCloseDisplay = int (*)(Display *);

    Impl();
    ~Impl();

    bool init();
    void finalize();

    bool connect();
    void disconnect();

private:
    DynamicLoadLibrary x_lib_dll_;

    FN_XOpenDisplay fn_x_open_display_;
    FN_XCloseDisplay fn_x_close_display_;

    Display *display_;
    int screen_;
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverX11::Impl::Impl() :
    fn_x_open_display_(nullptr),
    fn_x_close_display_(nullptr),
    display_(nullptr),
    screen_(-1)
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
    fn_x_close_display_ =
        (FN_XCloseDisplay)x_lib_dll_.findSymbol("XCloseDisplay");
    if (nullptr == fn_x_close_display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "failed to find symbol XCloseDisplay in X lib");
        return false;
    }

    return true;
}

void DisplayDriverX11::Impl::finalize()
{
    disconnect();

    fn_x_close_display_ = nullptr;
    fn_x_open_display_ = nullptr;

    x_lib_dll_.unload();
}

bool DisplayDriverX11::Impl::connect()
{
    if (nullptr == fn_x_open_display_) {
        return false;
    }

    display_ = fn_x_open_display_(nullptr);
    if (nullptr == display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "failed to open x11 display");
        return false;
    }
    screen_ = DefaultScreen(display_);

    return true;
}

void DisplayDriverX11::Impl::disconnect()
{
    if (nullptr == fn_x_close_display_) {
        return;
    }

    if (display_ != nullptr) {
        fn_x_close_display_(display_);
        display_ = nullptr;
        screen_ = -1;
    }
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

bool DisplayDriverX11::connect()
{
    return pimpl_->connect();
}

void DisplayDriverX11::disconnect()
{
    pimpl_->disconnect();
}

} // namespace brickred::moment::display
