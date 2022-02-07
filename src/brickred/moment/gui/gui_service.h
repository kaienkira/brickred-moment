#ifndef BRICKRED_MOMENT_GUI_WINDOW_H
#define BRICKRED_MOMENT_GUI_WINDOW_H

#include <memory>
#include <brickred/moment/base/class_util.h>

namespace brickred::moment::gui {

class GuiService {
public:
    bool init();
    void finalize();

private:
    BRICKRED_MOMENT_SINGLETON(GuiService)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::gui

#endif
