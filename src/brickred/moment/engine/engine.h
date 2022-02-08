#ifndef BRICKRED_MOMENT_ENGINE_ENGINE_H
#define BRICKRED_MOMENT_ENGINE_ENGINE_H

#include <memory>
#include <brickred/moment/base/class_util.h>

namespace brickred::moment::engine {

class Engine {
public:
    bool init();
    void finalize();
    void step();

private:
    BRICKRED_MOMENT_SINGLETON(Engine)

    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace brickred::moment::engine

#endif
