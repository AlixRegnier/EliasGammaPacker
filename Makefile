CPPFLAGS=-O3 -std=c++17 -pedantic -march=native -mavx2 -Wall -I./include #-DWRITE_RUNS
DEBUG_FLAGS=-O0 -g -std=c++17 -march=native -mavx2 -Wall -I./include #-DWRITE_RUNS
BENCH_FLAGS=-O3 -std=c++17 -march=native -mavx2 -Wall -g -fno-omit-frame-pointer -I./include #-DWRITE_RUNS

.PHONY: main debug bench clean test

main: lib rle_egp unrle_egp

debug: lib debug_rle_egp debug_unrle_egp

bench: lib bench_rle_egp bench_unrle_egp

lib:
	@mkdir -p ./lib

rle_egp: lib/egprle.o src/rle_egp.cpp
	g++ ${CPPFLAGS} -o rle_egp src/rle_egp.cpp lib/egprle.o

unrle_egp: lib/egprle.o src/unrle_egp.cpp
	g++ ${CPPFLAGS} -o unrle_egp src/unrle_egp.cpp lib/egprle.o

debug_rle_egp: lib/debug_egprle.o src/rle_egp.cpp
	g++ ${DEBUG_FLAGS} -o debug_rle_egp src/rle_egp.cpp lib/debug_egprle.o

debug_unrle_egp: lib/debug_egprle.o src/unrle_egp.cpp
	g++ ${DEBUG_FLAGS} -o debug_unrle_egp src/unrle_egp.cpp lib/debug_egprle.o

bench_rle_egp: lib/bench_egprle.o src/rle_egp.cpp
	g++ ${BENCH_FLAGS} -o bench_rle_egp src/rle_egp.cpp lib/bench_egprle.o

bench_unrle_egp: lib/bench_egprle.o src/unrle_egp.cpp
	g++ ${BENCH_FLAGS} -o bench_unrle_egp src/unrle_egp.cpp lib/bench_egprle.o

lib/egprle.o: include/egprle.h src/egprle.cpp include/circular_buffer.h include/utils.h include/types.h
	g++ ${CPPFLAGS} -c src/egprle.cpp -o lib/egprle.o

lib/debug_egprle.o:  include/egprle.h src/egprle.cpp include/circular_buffer.h include/utils.h include/types.h
	g++ ${DEBUG_FLAGS} -c src/egprle.cpp -o lib/debug_egprle.o

lib/bench_egprle.o:  include/egprle.h src/egprle.cpp include/circular_buffer.h include/utils.h include/types.h
	g++ ${BENCH_FLAGS} -c src/egprle.cpp -o lib/bench_egprle.o

clean:
	rm -f ./lib/*.o

test: ./test/test.cpp
	g++ ${CPPFLAGS} -o test_exec ./test/test.cpp
	./test_exec
	@rm -f test_exec

