#include <brickred/moment/display/display_service.h>

namespace brickred::moment::display {

class DisplayService::Impl {
public:
    Impl();
    ~Impl();

    bool init();
    void finalize();

private:
};

///////////////////////////////////////////////////////////////////////////////
DisplayService::Impl::Impl()
{
}

DisplayService::Impl::~Impl()
{
}

bool DisplayService::Impl::init()
{
    return true;
}

void DisplayService::Impl::finalize()
{
}

///////////////////////////////////////////////////////////////////////////////
DisplayService::DisplayService() :
    pimpl_(new Impl())
{
}

DisplayService::~DisplayService()
{
}

bool DisplayService::init()
{
    return pimpl_->init();
}

void DisplayService::finalize()
{
    pimpl_->finalize();
}

} // namespace brickred::moment::display
