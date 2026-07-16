RELEASE_FLAGS=-O3 -std=c++17 -pedantic -march=native -mavx2 -Wall -I./include #-DWRITE_RUNS
DEBUG_FLAGS=-O0 -g -std=c++17 -march=native -mavx2 -Wall -I./include #-DWRITE_RUNS
BENCH_FLAGS=-O3 -std=c++17 -march=native -mavx2 -Wall -g -fno-omit-frame-pointer -I./include #-DWRITE_RUNS

.PHONY: clean usage bench all

BUILD ?= release
BUILD_LOWER := $(shell echo "$(BUILD)" | tr A-Z a-z)

ifeq ($(BUILD_LOWER),release)
CFLAGS := $(RELEASE_FLAGS)
OBJ := lib/egprle.o
BIN :=
endif

ifeq ($(BUILD_LOWER),debug)
CFLAGS := $(DEBUG_FLAGS)
OBJ := lib/debug_egprle.o
BIN := debug_
endif

ifeq ($(BUILD_LOWER),bench)
CFLAGS := $(BENCH_FLAGS)
OBJ := lib/bench_egprle.o
BIN := bench_
endif

HEADERS := include/egprle.h \
           include/circular_buffer.h \
           include/utils.h \
           include/types.h

all: $(BIN)rle_egp $(BIN)unrle_egp
	@echo "Build target: $(BUILD_LOWER)"

$(BIN)rle_egp: $(OBJ) src/rle_egp.cpp
	$(CXX) $(CFLAGS) -o $@ $^

$(BIN)unrle_egp: $(OBJ) src/unrle_egp.cpp
	$(CXX) $(CFLAGS) -o $@ $^

$(OBJ): src/egprle.cpp $(HEADERS) | lib
	$(CXX) $(CFLAGS) -c $< -o $@

lib:
	@mkdir -p lib

clean:
	rm -f ./lib/*.o
	rm -f ./*rle_egp*

usage:
	@echo "Usage: make BUILD=[RELEASE|DEBUG|BENCH]"