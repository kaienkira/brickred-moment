#include <brickred/moment/display/display_driver_windows.h>

#include <cstring>
#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <windows.h>

#include <brickred/moment/base/internal_logger.h>

#define BRICKRED_MOMENT_WINDOW_CLASS_NAME L"BrickredMoment"

namespace brickred::moment::display {

class DisplayDriverWindows::Impl {
public:
    Impl();
    ~Impl();

    bool init();
    void finalize();

    bool createMainWindow(
        int32_t pos_x, int32_t pos_y,
        uint32_t width, uint32_t height);
    void pollEvents(bool block);

private:
    static LRESULT windowProc(
        HWND hwnd, UINT id,
        WPARAM wparam, LPARAM lparam);

private:
    bool quit_;
    HWND main_window_;
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverWindows::Impl::Impl() :
    main_window_(nullptr)
{
}

DisplayDriverWindows::Impl::~Impl()
{
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
            "windows: failed to register window class");
        return false;
    }

    return true;
}

void DisplayDriverWindows::Impl::finalize()
{
}

bool DisplayDriverWindows::Impl::createMainWindow(
    int32_t pos_x, int32_t pos_y,
    uint32_t width, uint32_t height)
{
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

    main_window_ = ::CreateWindowExW(
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

    return true;
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
LRESULT DisplayDriverWindows::Impl::windowProc(
    HWND hwnd, UINT id,
    WPARAM wparam, LPARAM lparam)
{
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

bool DisplayDriverWindows::createMainWindow(
    int32_t pos_x, int32_t pos_y,
    uint32_t width, uint32_t height)
{
    return pimpl_->createMainWindow(pos_x, pos_y, width, height);
}

void DisplayDriverWindows::pollEvents(bool block)
{
    pimpl_->pollEvents(block);
}

} // namespace brickred::moment::display
