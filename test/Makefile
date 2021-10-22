DIR_CURRENT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

all:
	for f in $(shell find $(DIR_CURRENT) -mindepth 2 -maxdepth 2  -type f -name Makefile -printf "%h\n") ; do \
		make -C $${f} test || exit 1; \
	done