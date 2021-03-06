#include <brickred/moment/display/display_driver_x11.h>

#include <unistd.h>
#include <X11/Xatom.h>
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

///////////////////////////////////////////////////////////////////////////////
class DynamicLoadLibraryX11 {
public:
    using FN_XOpenDisplay = Display *(*)(
        const char * // display_name
    );
    using FN_XCloseDisplay = int (*)(
        Display * // display
    );
    using FN_XFlush = int (*)(
        Display * // display
    );
    using FN_XCreateColormap = Colormap (*)(
        Display *, // display
        Window,    // w
        Visual *,  // visual
        int        // alloc
    );
    using FN_XCreateWindow = Window (*)(
        Display *,             // display
        Window,                // parent
        int,                   // x
        int,                   // y
        unsigned int,          // width
        unsigned int,          // height
        unsigned int,          // border_width
        int,                   // depth
        unsigned int,          // class
        Visual *,              // visual
        unsigned long,         // valuemask
        XSetWindowAttributes * // attributes
    );
    using FN_XMapWindow = int (*)(
        Display *, // display
        Window     // w
    );
    using FN_XInternAtom = Atom (*)(
        Display *,    // display
        const char *, // atom_name
        Bool          // only_if_exists
    );
    using FN_XChangeProperty = int (*)(
        Display *,             // display
        Window,                // w
        Atom,                  // property
        Atom,                  // type
        int,                   // format
        int,                   // mode
        const unsigned char *, // data
        int                    // nelements
    );
    using FN_XSetWMProtocols = Status (*)(
        Display *, // display
        Window,    // w
        Atom *,    // protocols
        int        // count
    );

public:
    DynamicLoadLibraryX11();
    ~DynamicLoadLibraryX11();

    bool load();
    void unload();

public:
    FN_XOpenDisplay fn_x_open_display;
    FN_XCloseDisplay fn_x_close_display;
    FN_XFlush fn_x_flush;
    FN_XCreateColormap fn_x_create_colormap;
    FN_XCreateWindow fn_x_create_window;
    FN_XMapWindow fn_x_map_window;
    FN_XInternAtom fn_x_intern_atom;
    FN_XChangeProperty fn_x_change_property;
    FN_XSetWMProtocols fn_x_set_wm_protocols;

private:
    DynamicLoadLibrary dll_;
};

///////////////////////////////////////////////////////////////////////////////
DynamicLoadLibraryX11::DynamicLoadLibraryX11() :
    fn_x_open_display(nullptr),
    fn_x_close_display(nullptr),
    fn_x_flush(nullptr),
    fn_x_create_colormap(nullptr),
    fn_x_create_window(nullptr),
    fn_x_map_window(nullptr),
    fn_x_intern_atom(nullptr),
    fn_x_change_property(nullptr),
    fn_x_set_wm_protocols(nullptr)
{
}

DynamicLoadLibraryX11::~DynamicLoadLibraryX11()
{
    unload();
}

bool DynamicLoadLibraryX11::load()
{
    if (dll_.load("libX11.so.6") == false) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to load libX11.so.6");
        return false;
    }

    // XOpenDisplay
    this->fn_x_open_display =
        (FN_XOpenDisplay)dll_.findSymbol("XOpenDisplay");
    if (nullptr == this->fn_x_open_display) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XOpenDisplay in X lib");
        return false;
    }
    // XCloseDisplay
    this->fn_x_close_display =
        (FN_XCloseDisplay)dll_.findSymbol("XCloseDisplay");
    if (nullptr == this->fn_x_close_display) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XCloseDisplay in X lib");
        return false;
    }
    // XFlush
    this->fn_x_flush =
        (FN_XFlush)dll_.findSymbol("XFlush");
    if (nullptr == this->fn_x_flush) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XFlush in X lib");
        return false;
    }
    // XCreateColormap
    this->fn_x_create_colormap =
        (FN_XCreateColormap)dll_.findSymbol("XCreateColormap");
    if (nullptr == this->fn_x_create_colormap) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XCreateColormap in X lib");
        return false;
    }
    // XCreateWindow
    this->fn_x_create_window =
        (FN_XCreateWindow)dll_.findSymbol("XCreateWindow");
    if (nullptr == this->fn_x_create_window) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XCreateWindow in X lib");
        return false;
    }
    // XMapWindow
    this->fn_x_map_window =
        (FN_XMapWindow)dll_.findSymbol("XMapWindow");
    if (nullptr == this->fn_x_map_window) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XMapWindow in X lib");
        return false;
    }
    // XInternAtom
    this->fn_x_intern_atom =
        (FN_XInternAtom)dll_.findSymbol("XInternAtom");
    if (nullptr == this->fn_x_intern_atom) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XInternAtom in X lib");
        return false;
    }
    // XChangeProperty
    this->fn_x_change_property =
        (FN_XChangeProperty)dll_.findSymbol("XChangeProperty");
    if (nullptr == this->fn_x_change_property) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XChangeProperty in X lib");
        return false;
    }
    // XSetWMProtocols
    this->fn_x_set_wm_protocols =
        (FN_XSetWMProtocols)dll_.findSymbol("XSetWMProtocols");
    if (nullptr == this->fn_x_set_wm_protocols) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to find symbol XSetWMProtocols in X lib");
        return false;
    }

    return true;
}

void DynamicLoadLibraryX11::unload()
{
    this->fn_x_set_wm_protocols = nullptr;
    this->fn_x_change_property = nullptr;
    this->fn_x_intern_atom = nullptr;
    this->fn_x_map_window = nullptr;
    this->fn_x_create_window = nullptr;
    this->fn_x_create_colormap = nullptr;
    this->fn_x_flush = nullptr;
    this->fn_x_close_display = nullptr;
    this->fn_x_open_display = nullptr;

    dll_.unload();
}

} // namespace

///////////////////////////////////////////////////////////////////////////////
class DisplayDriverX11::Impl {
public:
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
    void initAtoms();
    void finalizeAtoms();

private:
    DynamicLoadLibraryX11 x11_dll_;
    bool init_success_;

    Display *display_;
    Atom atom_wm_delete_window_;
    Atom atom_net_wm_pid_;
    Atom atom_net_wm_ping_;

    WindowDataMap windows_;
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverX11::Impl::Impl() :
    init_success_(false),
    display_(nullptr),
    atom_wm_delete_window_(0),
    atom_net_wm_pid_(0),
    atom_net_wm_ping_(0)
{
}

DisplayDriverX11::Impl::~Impl()
{
    finalize();
}

bool DisplayDriverX11::Impl::init()
{
    if (x11_dll_.load() == false) {
        return false;
    }

    init_success_ = true;

    return true;
}

void DisplayDriverX11::Impl::finalize()
{
    init_success_ = false;

    disconnect();

    x11_dll_.unload();
}

bool DisplayDriverX11::Impl::connect()
{
    if (init_success_ == false) {
        return false;
    }

    display_ = x11_dll_.fn_x_open_display(nullptr);
    if (nullptr == display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to open display");
        return false;
    }

    initAtoms();

    return true;
}

void DisplayDriverX11::Impl::disconnect()
{
    if (init_success_ == false) {
        return;
    }

    if (display_ != nullptr) {
        finalizeAtoms();
        x11_dll_.fn_x_close_display(display_);
        display_ = nullptr;
    }
}

bool DisplayDriverX11::Impl::createWindow(
    int32_t window_id,
    int32_t pos_x, int32_t pos_y,
    uint32_t width, uint32_t height)
{
    if (init_success_ == false) {
        return false;
    }

    if (nullptr == display_) {
        return false;
    }

    int screen = DefaultScreen(display_);
    Window root_window = RootWindow(display_, screen);
    Visual *visual = DefaultVisual(display_, screen);
    int depth = DefaultDepth(display_, screen);

    // create colormap
    Colormap colormap = x11_dll_.fn_x_create_colormap(
        display_, root_window, visual, AllocNone);

    // create window
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
    unsigned long value_mask = CWBorderPixel | CWColormap | CWEventMask;
    Window window_handler = x11_dll_.fn_x_create_window(
        display_, root_window, pos_x, pos_y, width, height,
        0, depth, InputOutput, visual, value_mask, &window_attrs);
    if (!window_handler) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_x11: failed to create main window");
        return false;
    }

    // declare the WM protocols supported
    {
        Atom wm_protocols[] = {
            atom_wm_delete_window_,
            atom_net_wm_ping_,
        };
        x11_dll_.fn_x_set_wm_protocols(
            display_, window_handler,
            wm_protocols,
            sizeof(wm_protocols) / sizeof(Atom));
    }

    // declare pid
    {
        const long pid = ::getpid();
        x11_dll_.fn_x_change_property(
            display_, window_handler, atom_net_wm_pid_,
            XA_CARDINAL, 32, PropModeReplace,
            (unsigned char *)&pid, 1);
    }

    // show window
    x11_dll_.fn_x_map_window(display_, window_handler);
    // flush
    x11_dll_.fn_x_flush(display_);

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
void DisplayDriverX11::Impl::initAtoms()
{
    atom_wm_delete_window_ = x11_dll_.fn_x_intern_atom(
        display_, "WM_DELETE_WINDOW", False);
    atom_net_wm_pid_ = x11_dll_.fn_x_intern_atom(
        display_, "_NET_WM_PID", False);
    atom_net_wm_ping_ = x11_dll_.fn_x_intern_atom(
        display_, "_NET_WM_PING", False);
}

void DisplayDriverX11::Impl::finalizeAtoms()
{
    atom_net_wm_ping_ = 0;
    atom_net_wm_pid_ = 0;
    atom_wm_delete_window_ = 0;
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
