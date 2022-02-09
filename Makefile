MAKE = make --no-print-directory

define ECHO
	@printf "\033[;31m"; printf $1; printf "\033[0m\n"
endef

.PHONY: \
build-linux-release \
clean

build-linux-release:
	@$(call ECHO, "[build linux release]")
	@$(MAKE) -f mak/libbrickredmoment_linux.mak release

clean:
	@$(MAKE) -f mak/libbrickredmoment_linux.mak clean
