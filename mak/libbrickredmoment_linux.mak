TARGET = build/linux/libbrickredmoment

LINK_TYPE = static
INCLUDE = -Isrc
BUILD_DIR = build/linux

include mak/libbrickredmoment.mak
include mak/main_linux.mak
