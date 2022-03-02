#ifndef BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_WINDOWS_H
#define BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_WINDOWS_H

#include <cstdint>
#include <memory>

#include <brickred/moment/base/class_util.h>

namespace brickred::moment::display {

class DisplayDriverWindows {
public:
    DisplayDriverWindows();
    ~DisplayDriverWindows();

    bool init();
    void finalize();

    bool createWindow(int32_t window_id,
        int32_t pos_x, int32_t pos_y,
        uint32_t width, uint32_t height);

private:
    BRICKRED_MOMENT_NONCOPYABLE(DisplayDriverWindows)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::display

#endif
