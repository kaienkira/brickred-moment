#ifndef BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_WAYLAND_H
#define BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_WAYLAND_H

#include <cstdint>
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

    bool createWindow(int32_t window_id,
        int32_t pos_x, int32_t pos_y,
        uint32_t width, uint32_t height);
    void deleteWindow(int32_t window_id);
    bool showWindow(int32_t window_id);

private:
    BRICKRED_MOMENT_NONCOPYABLE(DisplayDriverWayland)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::display

#endif
