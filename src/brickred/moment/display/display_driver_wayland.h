#ifndef BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_WAYLAND_H
#define BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_WAYLAND_H

#include <memory>

#include <brickred/moment/base/class_util.h>

namespace brickred::moment::display {

class DisplayDriverWayland {
public:
    DisplayDriverWayland();
    ~DisplayDriverWayland();

    bool init();
    void finalize();
    bool connect();
    void disconnect();

private:
    BRICKRED_MOMENT_NONCOPYABLE(DisplayDriverWayland)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::display

#endif
