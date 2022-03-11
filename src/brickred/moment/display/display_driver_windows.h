#ifndef BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_WINDOWS_H
#define BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_WINDOWS_H

#include <cstdint>
#include <memory>

#include <brickred/moment/base/class_util.h>

namespace brickred::moment::display {

class DisplayDriverWindows {
public:
    bool init();
    void finalize();

    bool createWindow(
        int32_t window_id,
        int32_t pos_x, int32_t pos_y,
        uint32_t width, uint32_t height);
    void destoryWindow(int32_t window_id);
    void destoryAllWindows();
    bool checkWindowShouldClose(int32_t window_id) const;
    void setWindowShouldClose(int32_t window_id, bool should_close);

    void pollEvents(bool block = false);

private:
    BRICKRED_MOMENT_SINGLETON(DisplayDriverWindows)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::display

#endif
