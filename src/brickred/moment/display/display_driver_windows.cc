#include <brickred/moment/display/display_driver_windows.h>

#include <windows.h>

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
};

DisplayDriverWindows::Impl::Impl()
{
}

DisplayDriverWindows::Impl::~Impl()
{
}

bool DisplayDriverWindows::Impl::init()
{
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
    return pimpl_->createWindow(pos_x, pos_y, width, height);
}

} // namespace brickred::moment::display
