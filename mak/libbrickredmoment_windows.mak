TARGET = build/windows/libbrickredmoment

LINK_TYPE = static
INCLUDE = -Isrc
CPP_FLAG = -DBRICKRED_MOMENT_BUILD_PLATFORM_WINDOWS
BUILD_DIR = build/windows

include mak/libbrickredmoment.mak
include mak/main_windows.mak
