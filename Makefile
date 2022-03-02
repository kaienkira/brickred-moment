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
$(error Platform $(UNAME_S) not Supported)
endif

build-linux:
	@$(call ECHO, "[build libbrickredmoment]")
	@$(MAKE) -f mak/libbrickredmoment_linux.mak release

clean-linux:
	@$(MAKE) -f mak/libbrickredmoment_linux.mak clean
