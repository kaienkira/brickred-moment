#ifndef BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_X11_H
#define BRICKRED_MOMENT_DISPLAY_DISPLAY_DRIVER_X11_H

#include <cstdint>
#include <memory>

#include <brickred/moment/base/class_util.h>

namespace brickred::moment::display {

class DisplayDriverX11 {
public:
    DisplayDriverX11();
    ~DisplayDriverX11();

    bool init();
    void finalize();

    bool connect();
    void disconnect();

    bool createWindow(int32_t window_id);
    bool showWindow(int32_t window_id);
    void deleteWindow(int32_t window_id);

private:
    BRICKRED_MOMENT_NONCOPYABLE(DisplayDriverX11)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::display

#endif
