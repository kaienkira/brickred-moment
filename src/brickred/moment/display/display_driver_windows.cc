#include <brickred/moment/display/display_driver_windows.h>

namespace brickred::moment::display {

class DisplayDriverWindows::Impl {
public:
    Impl();
    ~Impl();

    bool init();
    void finalize();

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

} // namespace brickred::moment::display
