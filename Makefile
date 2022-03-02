MAKE = make --no-print-directory

define ECHO
	@printf "\033[;31m"; printf $1; printf "\033[0m\n"
endef

.PHONY: \
build clean \
build-linux clean-linux \
build-windows clean-windows

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
build: build-linux
clean: clean-linux
else ifeq ($(OS), Windows_NT)
build: build-windows
clean: clean-windows
else
$(error Platform not Supported)
endif

build-linux:
	@$(call ECHO, "[build libbrickredmoment]")
	@mkdir -p build/linux/
	@$(MAKE) -f mak/libbrickredmoment_linux.mak release

clean-linux:
	@rm -rf build/linux/

build-windows:
	@$(call ECHO, "[build libbrickredmoment]")
	@mkdir -p build/windows/
	@$(MAKE) -f mak/libbrickredmoment_windows.mak release

clean-windows:
	@rm -rf build/windows/
