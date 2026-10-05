CC ?= gcc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=200809L
CPPFLAGS := -Iinclude
BIN := bin/snapstore
SRC := src/snapstore.c src/crc32.c

.PHONY: all clean test demo driver
all: $(BIN)
$(BIN): $(SRC) include/snapstore.h include/snapmon_ioctl.h
	@mkdir -p bin
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SRC) -o $@

test: $(BIN)
	bash tests/test_engine.sh

demo: $(BIN)
	bash scripts/demo.sh

driver:
	$(MAKE) -C driver

clean:
	rm -rf bin build/demo-store build/test-store build/*.out build/*.txt
	-$(MAKE) -C driver clean
