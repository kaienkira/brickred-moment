TARGET = build/linux/libbrickredmoment

LINK_TYPE = static
INCLUDE = -Isrc
CPP_FLAG = -DBRICKRED_MOMENT_BUILD_PLATFORM_LINUX
BUILD_DIR = build/linux

include mak/libbrickredmoment.mak
include mak/main_linux.mak
