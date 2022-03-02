TARGET = build/windows/libbrickredmoment

LINK_TYPE = static
INCLUDE = -Isrc
CPP_FLAG = -DBRICKRED_MOMENT_BUILD_PLATFORM_WINDOWS
BUILD_DIR = build/windows

include mak/libbrickredmoment.mak

SRCS += \
src/brickred/moment/display/display_driver_windows.cc \

include mak/main_windows.mak
