#include <brickred/moment/display/display_driver_wayland.h>

#include <wayland-client-core.h>

#include <brickred/moment/base/dynamic_load_library.h>
#include <brickred/moment/base/internal_logger.h>

namespace brickred::moment::display {

using brickred::moment::base::DynamicLoadLibrary;

class DisplayDriverWayland::Impl {
public:
    using FN_wl_display_connect = wl_display *(*)(const char *);
    using FN_wl_display_disconnect = void (*)(wl_display *);
    using FN_wl_display_flush = int (*)(wl_display *);
    using FN_wl_proxy_marshal_flags = wl_proxy *(*)(
        wl_proxy *, uint32_t, const wl_interface*,
        uint32_t, uint32_t, ...);

    Impl();
    ~Impl();

    bool init();
    void finalize();
    bool connect();
    void disconnect();

private:
    DynamicLoadLibrary wayland_client_dll_;

    FN_wl_display_connect fn_wl_display_connect_;
    FN_wl_display_disconnect fn_wl_display_disconnect_;
    FN_wl_display_flush fn_wl_display_flush_;
    FN_wl_proxy_marshal_flags fn_wl_proxy_marshal_flags_;

    wl_display *display_;
};

///////////////////////////////////////////////////////////////////////////////
DisplayDriverWayland::Impl::Impl() :
    fn_wl_display_connect_(nullptr),
    fn_wl_display_disconnect_(nullptr),
    fn_wl_display_flush_(nullptr),
    fn_wl_proxy_marshal_flags_(nullptr),
    display_(nullptr)
{
}

DisplayDriverWayland::Impl::~Impl()
{
    finalize();
}

bool DisplayDriverWayland::Impl::init()
{
    if (wayland_client_dll_.load("libwayland-client.so.0") == false) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "wayland: failed to load libwayland-client.so.0");
        return false;
    }

    // wl_display_connect
    fn_wl_display_connect_ =
        (FN_wl_display_connect)wayland_client_dll_.findSymbol(
            "wl_display_connect");
    if (nullptr == fn_wl_display_connect_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "wayland: failed to find symbol wl_display_connect "
            "in wayland-client lib");
        return false;
    }
    // wl_display_disconnect
    fn_wl_display_disconnect_ =
        (FN_wl_display_disconnect)wayland_client_dll_.findSymbol(
            "wl_display_disconnect");
    if (nullptr == fn_wl_display_disconnect_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "wayland: failed to find symbol wl_display_disconnect"
            "in wayland-client lib");
        return false;
    }
    // wl_display_flush
    fn_wl_display_flush_ =
        (FN_wl_display_flush)wayland_client_dll_.findSymbol(
            "wl_display_flush");
    if (nullptr == fn_wl_display_flush_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "wayland: failed to find symbol wl_display_flush"
            "in wayland-client lib");
        return false;
    }
    // wl_proxy_marshal_flags
    fn_wl_proxy_marshal_flags_ =
        (FN_wl_proxy_marshal_flags)wayland_client_dll_.findSymbol(
            "wl_proxy_marshal_flags");
    if (nullptr == fn_wl_proxy_marshal_flags_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "wayland: failed to find symbol wl_proxy_marshal_flags"
            "in wayland-client lib");
        return false;
    }

    return true;
}

void DisplayDriverWayland::Impl::finalize()
{
    disconnect();

    fn_wl_proxy_marshal_flags_ = nullptr;
    fn_wl_display_flush_ = nullptr;
    fn_wl_display_disconnect_ = nullptr;
    fn_wl_display_connect_ = nullptr;

    wayland_client_dll_.unload();
}

bool DisplayDriverWayland::Impl::connect()
{
    if (nullptr == fn_wl_display_connect_) {
        return false;
    }

    display_ = fn_wl_display_connect_(nullptr);
    if (nullptr == display_) {
        BRICKRED_MOMENT_INTERNAL_LOG_ERROR(
            "wayland: failed to open display");
        return false;
    }

    return true;
}

void DisplayDriverWayland::Impl::disconnect()
{
    if (display_ != nullptr) {
        if (fn_wl_display_disconnect_ != nullptr) {
            fn_wl_display_disconnect_(display_);
        }
        display_ = nullptr;
    }
}

///////////////////////////////////////////////////////////////////////////////
DisplayDriverWayland::DisplayDriverWayland() :
    pimpl_(new Impl())
{
}

DisplayDriverWayland::~DisplayDriverWayland()
{
}

bool DisplayDriverWayland::init()
{
    return pimpl_->init();
}

void DisplayDriverWayland::finalize()
{
    pimpl_->finalize();
}

bool DisplayDriverWayland::connect()
{
    return pimpl_->connect();
}

void DisplayDriverWayland::disconnect()
{
    pimpl_->disconnect();
}

} // namespace brickred::moment::display
