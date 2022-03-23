#include <brickred/moment/display/display_driver_x11.h>

#include <X11/Xlib.h>
#include <map>

#include <brickred/moment/base/dynamic_load_library.h>
#include <brickred/moment/base/internal_logger.h>

namespace brickred::moment::display {

using brickred::moment::base::DynamicLoadLibrary;

namespace {

class WindowData {
public:
    WindowData(Window handler);
    ~WindowData();

    Window getHandler() { return handler_; }
    bool checkShouldClose() const { return should_close_; }
    void setShouldClose(bool should_close) { should_close_ = should_close; }

private:
    Window handler_;
    bool should_close_;
};

///////////////////////////////////////////////////////////////////////////////
WindowData::WindowData(Window handler) :
    handler_(handler),
    should_close_(false)
{
}

WindowData::~WindowData()
{
}

} // namespace

///////////////////////////////////////////////////////////////////////////////
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
    using FN_XInternAtom = Atom (*)(
        Display *, const char *, Bool);

    using WindowDataMap = std::map<int32_t, WindowData *>;

    Impl();
    ~Impl();

    bool init();
    void finalize();
    bool connect();
    void disconnect();

    bool createWindow(
        int32_t window_id,
        int32_t pos_x, int32_t pos_y,
        uint32_t width, uint32_t height);
    void destoryWindow(int32_t window_id);
    void destoryAllWindows();
    bool checkWindowShouldClose(int32_t window_id) const;
    void setWindowShouldClose(int32_t window_id, bool should_close);

    void pollEvents(bool block);

private:
    bool loadXlibDll();
    void unloadXlibDll();

private:
    DynamicLoadLibrary x_lib_dll_;
    FN_XOpenDisplay fn_x_open_display_;
    FN_XCloseDisplay fn_x_close_display_;
    FN_XCreateColormap fn_x_create_colormap_;
    FN_XCreateWindow fn_x_create_window_;
    FN_XInternAtom fn_x_intern_atom_;

    Display *display_;
    WindowDataMap windows_;
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverX11::Impl::Impl() :
    fn_x_open_display_(nullptr),
    fn_x_close_display_(nullptr),
    fn_x_create_colormap_(nullptr),
    fn_x_create_window_(nullptr),
    fn_x_intern_atom_(nullptr),
    display_(nullptr)
{
}

DisplayDriverX11::Impl::~Impl()
{
    finalize();
}

bool DisplayDriverX11::Impl::init()
{
    if (loadXlibDll() == false) {
        return false;
    }

    return true;
}

void DisplayDriverX11::Impl::finalize()
{
    disconnect();

    unloadXlibDll();
}

bool DisplayDriverX11::Impl::connect()
{
    if (nullptr == fn_x_open_display_) {
        return false;
    }

    display_ = fn_x_open_display_(nullptr);
    if (nullptr == display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to open display");
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

bool DisplayDriverX11::Impl::createWindow(
    int32_t window_id,
    int32_t pos_x, int32_t pos_y,
    uint32_t width, uint32_t height)
{
    if (nullptr == display_) {
        return false;
    }
    if (nullptr == fn_x_create_colormap_ ||
        nullptr == fn_x_create_window_) {
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

    Window window_handler = fn_x_create_window_(
        display_, root_window, pos_x, pos_y, width, height,
        0, depth, InputOutput, visual, value_mask, &window_attrs);
    if (!window_handler) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to create main window");
        return false;
    }

    std::unique_ptr<WindowData> window(
        new WindowData(window_handler));
    windows_.insert(std::make_pair(window_id, window.get()));
    window.release();

    return true;
}

void DisplayDriverX11::Impl::destoryWindow(int32_t window_id)
{
}

void DisplayDriverX11::Impl::destoryAllWindows()
{
}

bool DisplayDriverX11::Impl::checkWindowShouldClose(
    int32_t window_id) const
{
    return true;
}

void DisplayDriverX11::Impl::setWindowShouldClose(
    int32_t window_id, bool should_close)
{
}

void DisplayDriverX11::Impl::pollEvents(bool block)
{
}

///////////////////////////////////////////////////////////////////////////////
bool DisplayDriverX11::Impl::loadXlibDll()
{
    if (x_lib_dll_.load("libX11.so.6") == false) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to load libX11.so.6");
        return false;
    }

    // XOpenDisplay
    fn_x_open_display_ =
        (FN_XOpenDisplay)x_lib_dll_.findSymbol("XOpenDisplay");
    if (nullptr == fn_x_open_display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XOpenDisplay in X lib");
        return false;
    }
    // XCloseDisplay
    fn_x_close_display_ =
        (FN_XCloseDisplay)x_lib_dll_.findSymbol("XCloseDisplay");
    if (nullptr == fn_x_close_display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XCloseDisplay in X lib");
        return false;
    }
    // XCreateColormap
    fn_x_create_colormap_ =
        (FN_XCreateColormap)x_lib_dll_.findSymbol("XCreateColormap");
    if (nullptr == fn_x_create_colormap_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XCreateColormap in X lib");
        return false;
    }
    // XCreateWindow
    fn_x_create_window_ =
        (FN_XCreateWindow)x_lib_dll_.findSymbol("XCreateWindow");
    if (nullptr == fn_x_create_window_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XCreateWindow in X lib");
        return false;
    }
    // XInternAtom
    fn_x_intern_atom_ =
        (FN_XInternAtom)x_lib_dll_.findSymbol("XInternAtom");
    if (nullptr == fn_x_intern_atom_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XInternAtom in X lib");
        return false;
    }

    return true;
}

void DisplayDriverX11::Impl::unloadXlibDll()
{
    fn_x_intern_atom_ = nullptr;
    fn_x_create_window_ = nullptr;
    fn_x_create_colormap_ = nullptr;
    fn_x_close_display_ = nullptr;
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

bool DisplayDriverX11::connect()
{
    return pimpl_->connect();
}

void DisplayDriverX11::disconnect()
{
    pimpl_->disconnect();
}

bool DisplayDriverX11::createWindow(
    int32_t window_id,
    int32_t pos_x, int32_t pos_y,
    uint32_t width, uint32_t height)
{
    return pimpl_->createWindow(window_id, pos_x, pos_y, width, height);
}

void DisplayDriverX11::destoryWindow(int32_t window_id)
{
    pimpl_->destoryWindow(window_id);
}

void DisplayDriverX11::destoryAllWindows()
{
    pimpl_->destoryAllWindows();
}

bool DisplayDriverX11::checkWindowShouldClose(
    int32_t window_id) const
{
    return pimpl_->checkWindowShouldClose(window_id);
}

void DisplayDriverX11::setWindowShouldClose(
    int32_t window_id, bool should_close)
{
    pimpl_->setWindowShouldClose(window_id, should_close);
}

void DisplayDriverX11::pollEvents(bool block)
{
    pimpl_->pollEvents(block);
}

} // namespace brickred::moment::display
