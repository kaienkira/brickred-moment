#include "brickred/moment/engine/engine.h"

namespace brickred::moment::engine {

class Engine::Impl {
public:
    Impl();
    ~Impl();
};

///////////////////////////////////////////////////////////////////////////////
Engine::Impl::Impl()
{
}

Engine::Impl::~Impl()
{
}

///////////////////////////////////////////////////////////////////////////////
Engine::Engine() :
    pimpl_(new Impl())
{
}

Engine::~Engine()
{
}

bool Engine::init()
{
    return true;
}

void Engine::finalize()
{
}

void Engine::step()
{
}

} // namespace brickred::moment::engine
