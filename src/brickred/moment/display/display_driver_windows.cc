#include <brickred/moment/display/display_driver_windows.h>

#include <cstring>
#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <windows.h>
#include <map>

#include <brickred/moment/base/internal_logger.h>

#define BRICKRED_MOMENT_WINDOW_CLASS_NAME L"BrickredMoment"

namespace brickred::moment::display {

namespace {

class Window {
public:
    Window(HWND handler);
    ~Window();

    HWND getHandler() { return handler_; }
    bool checkShouldClose() const { return should_close_; }
    void setShouldClose(bool should_close) { should_close_ = should_close; }

private:
    HWND handler_;
    bool should_close_;
};

///////////////////////////////////////////////////////////////////////////////
Window::Window(HWND handler) :
    handler_(handler),
    should_close_(false)
{
}

Window::~Window()
{
}

} // namespace

///////////////////////////////////////////////////////////////////////////////
class DisplayDriverWindows::Impl {
public:
    using WindowMap = std::map<int32_t, Window *>;
    using WindowHandleIndex = std::map<HWND, Window *>;

    Impl();
    ~Impl();

    bool init();
    void finalize();

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
    const Window *findWindowById(int32_t window_id) const;
    Window *findWindowById(int32_t window_id);
    Window *findWindowByHandler(HWND window_handler);
    void destoryWindow(WindowMap::iterator iter);

private:
    static LRESULT windowProc(
        HWND hwnd, UINT id,
        WPARAM wparam, LPARAM lparam);
    LRESULT doWindowProc(
        HWND hwnd, UINT id,
        WPARAM wparam, LPARAM lparam);

private:
    WindowMap windows_;
    WindowHandleIndex window_handler_index_;
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverWindows::Impl::Impl()
{
}

DisplayDriverWindows::Impl::~Impl()
{
    finalize();
}

bool DisplayDriverWindows::Impl::init()
{
    // register window class
    WNDCLASSEXW wc;
    ::memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)DisplayDriverWindows::Impl::windowProc;
    wc.hInstance = ::GetModuleHandleW(nullptr);
    wc.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = BRICKRED_MOMENT_WINDOW_CLASS_NAME;
    if (RegisterClassExW(&wc) == 0) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_windows: failed to register window class");
        return false;
    }

    return true;
}

void DisplayDriverWindows::Impl::finalize()
{
    destoryAllWindows();
}

bool DisplayDriverWindows::Impl::createWindow(
    int32_t window_id,
    int32_t pos_x, int32_t pos_y,
    uint32_t width, uint32_t height)
{
    if (windows_.find(window_id) != windows_.end()) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_windows: window(%d) already exists", window_id);
        return false;
    }

    DWORD style = WS_CLIPSIBLINGS |
                  WS_CLIPCHILDREN |
                  WS_OVERLAPPEDWINDOW |
                  WS_VISIBLE;
    DWORD ex_style = WS_EX_APPWINDOW;

    if (pos_x < 0) {
        pos_x = CW_USEDEFAULT;
    }
    if (pos_y < 0) {
        pos_y = CW_USEDEFAULT;
    }

    HWND window_handler = ::CreateWindowExW(
        ex_style,
        BRICKRED_MOMENT_WINDOW_CLASS_NAME,
        L"",
        style,
        pos_x, pos_y,
        width, height,
        nullptr, // hWndParent
        nullptr, // hMenu
        ::GetModuleHandleW(nullptr),
        nullptr);
    if (nullptr == window_handler) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_windows: failed to create window");
        return false;
    }

    std::unique_ptr<Window> window(new Window(window_handler));
    windows_.insert(std::make_pair(window_id, window.get()));
    Window *window_raw = window.release();
    window_handler_index_.insert(std::make_pair(
        window_raw->getHandler(), window_raw));

    return true;
}

void DisplayDriverWindows::Impl::destoryWindow(int32_t window_id)
{
    WindowMap::iterator iter = windows_.find(window_id);
    if (iter == windows_.end()) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_windows: window(%d) not found", window_id);
        return;
    }

    destoryWindow(iter);
}

void DisplayDriverWindows::Impl::destoryAllWindows()
{
    for (WindowMap::iterator iter = windows_.begin();
         iter != windows_.end();) {
        destoryWindow(iter++);
    }
}

bool DisplayDriverWindows::Impl::checkWindowShouldClose(
    int32_t window_id) const
{
    const Window *window = findWindowById(window_id);
    if (nullptr == window) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_windows: window(%d) not found", window_id);
        return false;
    }

    return window->checkShouldClose();
}

void DisplayDriverWindows::Impl::setWindowShouldClose(
    int32_t window_id, bool should_close)
{
    Window *window = findWindowById(window_id);
    if (nullptr == window) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "display_windows: window(%d) not found", window_id);
        return;
    }

    window->setShouldClose(should_close);
}

void DisplayDriverWindows::Impl::pollEvents(bool block)
{
    MSG msg;

    if (block) {
        ::WaitMessage();
    }
    while (::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
    }
}

///////////////////////////////////////////////////////////////////////////////
const Window *DisplayDriverWindows::Impl::findWindowById(
    int32_t window_id) const
{
    WindowMap::const_iterator iter = windows_.find(window_id);
    if (iter == windows_.end()) {
        return nullptr;
    }

    return iter->second;
}

Window *DisplayDriverWindows::Impl::findWindowById(int32_t window_id)
{
    WindowMap::iterator iter = windows_.find(window_id);
    if (iter == windows_.end()) {
        return nullptr;
    }

    return iter->second;
}

Window *DisplayDriverWindows::Impl::findWindowByHandler(HWND window_handler)
{
    WindowHandleIndex::iterator iter =
        window_handler_index_.find(window_handler);
    if (iter == window_handler_index_.end()) {
        return nullptr;
    }

    return iter->second;
}

void DisplayDriverWindows::Impl::destoryWindow(WindowMap::iterator iter)
{
    Window *window = iter->second;
    window_handler_index_.erase(window->getHandler());
    windows_.erase(iter);
    ::DestroyWindow(window->getHandler());
    delete window;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT DisplayDriverWindows::Impl::windowProc(
    HWND hwnd, UINT id,
    WPARAM wparam, LPARAM lparam)
{
    return DisplayDriverWindows::getInstance()->pimpl_->doWindowProc(
        hwnd, id, wparam, lparam);
}

LRESULT DisplayDriverWindows::Impl::doWindowProc(
    HWND hwnd, UINT id,
    WPARAM wparam, LPARAM lparam)
{
    if (WM_CLOSE == id) {
        Window *window = findWindowByHandler(hwnd);
        if (window != nullptr) {
            window->setShouldClose(true);
            return 0;
        }
    }

    return ::DefWindowProcW(hwnd, id, wparam, lparam);
}

///////////////////////////////////////////////////////////////////////////////
DisplayDriverWindows::DisplayDriverWindows() :
    pimpl_(new Impl())
{
}

DisplayDriverWindows::~DisplayDriverWindows()
{
}

bool DisplayDriverWindows::init()
{
    return pimpl_->init();
}

void DisplayDriverWindows::finalize()
{
    pimpl_->finalize();
}

bool DisplayDriverWindows::createWindow(
    int32_t window_id,
    int32_t pos_x, int32_t pos_y,
    uint32_t width, uint32_t height)
{
    return pimpl_->createWindow(window_id, pos_x, pos_y, width, height);
}

void DisplayDriverWindows::destoryWindow(int32_t window_id)
{
    pimpl_->destoryWindow(window_id);
}

void DisplayDriverWindows::destoryAllWindows()
{
    pimpl_->destoryAllWindows();
}

bool DisplayDriverWindows::checkWindowShouldClose(int32_t window_id) const
{
    return pimpl_->checkWindowShouldClose(window_id);
}

void DisplayDriverWindows::setWindowShouldClose(
    int32_t window_id, bool should_close)
{
    pimpl_->setWindowShouldClose(window_id, should_close);
}

void DisplayDriverWindows::pollEvents(bool block)
{
    pimpl_->pollEvents(block);
}

} // namespace brickred::moment::display
