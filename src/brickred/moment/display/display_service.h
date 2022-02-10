#ifndef BRICKRED_MOMENT_DISPLAY_DISPLAY_SERVICE_H
#define BRICKRED_MOMENT_DISPLAY_DISPLAY_SERVICE_H

#include <memory>
#include <brickred/moment/base/class_util.h>

namespace brickred::moment::display {

class DisplayService {
public:

private:
    BRICKRED_MOMENT_SINGLETON(DisplayService)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::display

#endif
