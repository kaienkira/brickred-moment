#include <brickred/moment/display/display_service.h>

#if defined(BRICKRED_MOMENT_BUILD_PLATFORM_LINUX)
#include "display_service_x11.cc"
#endif

namespace brickred::moment::display {

DisplayService::DisplayService() :
    pimpl_(new Impl())
{
}

DisplayService::~DisplayService()
{
}

} // namespace brickred::moment::display
