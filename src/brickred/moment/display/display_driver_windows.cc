#include <brickred/moment/display/display_driver_windows.h>

#include <cstring>
#define WIN32_LEAN_AND_MEAN
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

private:
    static LRESULT windowProc(HWND hWnd, UINT uMsg,
        WPARAM wParam, LPARAM lParam);

private:
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
    DWORD style = 0;
    DWORD ex_style = 0;

    return true;
}

LRESULT DisplayDriverWindows::Impl::windowProc(
    HWND hWnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
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

} // namespace brickred::moment::display
