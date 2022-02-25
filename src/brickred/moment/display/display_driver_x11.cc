#include <brickred/moment/display/display_driver_x11.h>

#include <X11/Xlib.h>
#include <map>

#include <brickred/moment/base/dynamic_load_library.h>
#include <brickred/moment/base/internal_logger.h>

namespace brickred::moment::display {

using brickred::moment::base::DynamicLoadLibrary;

class DisplayDriverX11::Impl {
public:
    using FN_XOpenDisplay = Display *(*)(const char *);
    using FN_XCloseDisplay = int (*)(Display *);
    using FN_XCreateColormap = Colormap (*)(
        Display *, Window, Visual *, int);
    using FN_XCreateWindow = Window (*)(
        Display *, Window, int, int,
        unsigned int, unsigned int, unsigned int, int, unsigned int,
        Visual *, unsigned long, XSetWindowAttributes *);
    using WindowMap = std::map<int32_t, Window>;

    Impl();
    ~Impl();

    bool init();
    void finalize();
    bool connect();
    void disconnect();

    bool createWindow(int32_t window_id,
        int32_t pos_x, int32_t pos_y, uint32_t width, uint32_t height);
    void deleteWindow(int32_t window_id);
    bool showWindow(int32_t window_id);

private:
    DynamicLoadLibrary x_lib_dll_;

    FN_XOpenDisplay fn_x_open_display_;
    FN_XCloseDisplay fn_x_close_display_;
    FN_XCreateColormap fn_x_create_colormap_;
    FN_XCreateWindow fn_x_create_window_;

    Display *display_;
    WindowMap windows_;
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverX11::Impl::Impl() :
    fn_x_open_display_(nullptr),
    fn_x_close_display_(nullptr),
    fn_x_create_colormap_(nullptr),
    fn_x_create_window_(nullptr),
    display_(nullptr)
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
            "x11: failed to load libX11.so.6");
        return false;
    }

    fn_x_open_display_ =
        (FN_XOpenDisplay)x_lib_dll_.findSymbol("XOpenDisplay");
    if (nullptr == fn_x_open_display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "x11: failed to find symbol XOpenDisplay in X lib");
        return false;
    }
    fn_x_close_display_ =
        (FN_XCloseDisplay)x_lib_dll_.findSymbol("XCloseDisplay");
    if (nullptr == fn_x_close_display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "x11: failed to find symbol XCloseDisplay in X lib");
        return false;
    }
    fn_x_create_colormap_ =
        (FN_XCreateColormap)x_lib_dll_.findSymbol("XCreateColormap");
    if (nullptr == fn_x_create_colormap_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "x11: failed to find symbol XCreateColormap in X lib");
        return false;
    }
    fn_x_create_window_ =
        (FN_XCreateWindow)x_lib_dll_.findSymbol("XCreateWindow");
    if (nullptr == fn_x_create_window_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "x11: failed to find symbol XCreateWindow in X lib");
        return false;
    }

    return true;
}

void DisplayDriverX11::Impl::finalize()
{
    disconnect();

    fn_x_create_window_ = nullptr;
    fn_x_create_colormap_ = nullptr;
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
            "x11: failed to open display");
        return false;
    }

    return true;
}

void DisplayDriverX11::Impl::disconnect()
{
    if (display_ != nullptr) {
        if (fn_x_close_display_ != nullptr) {
            fn_x_close_display_(display_);
        }
        display_ = nullptr;
    }
}

bool DisplayDriverX11::Impl::createWindow(int32_t window_id,
    int32_t pos_x, int32_t pos_y, uint32_t width, uint32_t height)
{
    if (nullptr == display_) {
        return false;
    }
    if (nullptr == fn_x_create_colormap_ ||
        nullptr == fn_x_create_window_) {
        return false;
    }

    if (windows_.find(window_id) != windows_.end()) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "x11: window(%d) already exists", window_id);
        return false;
    }

    int screen = DefaultScreen(display_);
    Visual *visual = DefaultVisual(display_, screen);
    int depth = DefaultDepth(display_, screen);
    Window root_window = RootWindow(display_, screen);
    Colormap colormap = fn_x_create_colormap_(
        display_, root_window, visual, AllocNone);
    unsigned long value_mask = CWBorderPixel | CWColormap | CWEventMask;

    XSetWindowAttributes window_attrs = { 0 };
    window_attrs.colormap = colormap;
    window_attrs.background_pixel = 0xffffffff;
    window_attrs.border_pixel = 0;
    window_attrs.event_mask =
        StructureNotifyMask | PropertyChangeMask |
        ExposureMask | FocusChangeMask | VisibilityChangeMask |
        KeyPressMask | KeyReleaseMask |
        ButtonPressMask | ButtonReleaseMask |
        EnterWindowMask | LeaveWindowMask | PointerMotionMask;

    Window new_window = fn_x_create_window_(
        display_, root_window, pos_x, pos_y, width, height,
        0, depth, InputOutput, visual, value_mask, &window_attrs);
    if (!new_window) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "x11: failed to create window");
        return false;
    }

    windows_.insert(std::make_pair(window_id, new_window));

    return true;
}

void DisplayDriverX11::Impl::deleteWindow(int32_t window_id)
{
}

bool DisplayDriverX11::Impl::showWindow(int32_t window_id)
{
    return true;
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

bool DisplayDriverX11::createWindow(int32_t window_id,
    int32_t pos_x, int32_t pos_y, uint32_t width, uint32_t height)
{
    return pimpl_->createWindow(window_id, pos_x, pos_y, width, height);
}

void DisplayDriverX11::deleteWindow(int32_t window_id)
{
    pimpl_->deleteWindow(window_id);
}

bool DisplayDriverX11::showWindow(int32_t window_id)
{
    return pimpl_->showWindow(window_id);
}

} // namespace brickred::moment::display
