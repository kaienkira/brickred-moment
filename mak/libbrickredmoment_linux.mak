TARGET = build/linux/libbrickredmoment

LINK_TYPE = static
INCLUDE = -Isrc
CPP_FLAG = -DBRICKRED_MOMENT_BUILD_PLATFORM_LINUX
BUILD_DIR = build/linux

include mak/libbrickredmoment.mak

SRCS += \
src/brickred/moment/display/display_driver_wayland.cc \
src/brickred/moment/display/display_driver_x11.cc \

include mak/main_linux.mak
