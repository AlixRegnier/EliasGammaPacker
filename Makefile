CPPFLAGS=-O3 -std=c++17 -pedantic -march=native -mavx2 -Wall #-DWRITE_RUNS
DEBUG_FLAGS=-O0 -g -std=c++17 -march=native -mavx2 -Wall #-DWRITE_RUNS
BENCH_FLAGS=-O3 -std=c++17 -march=native -mavx2 -Wall -g -fno-omit-frame-pointer

.PHONY: all debug bench clean test

all: rle_egp unrle_egp

debug: debug_rle_egp debug_unrle_egp

bench: bench_rle_egp bench_unrle_egp
	flamegraph ./bench_rle_egp ./data/matrix_148_senterica_reordered.cmbf ./tmp/senterica.rle
	mv ./flamegraph.svg ./flamegraph_rle.svg

	flamegraph ./bench_unrle_egp ./tmp/senterica.rle ./tmp/senterica.cmbf
	mv ./flamegraph.svg ./flamegraph_unrle.svg

rle_egp: egprle.o rle_egp.cpp
	g++ ${CPPFLAGS} -o rle_egp  rle_egp.cpp egprle.o

unrle_egp: egprle.o unrle_egp.cpp
	g++ ${CPPFLAGS} -o unrle_egp unrle_egp.cpp egprle.o

debug_rle_egp: debug_egprle.o rle_egp.cpp
	g++ ${DEBUG_FLAGS} -o debug_rle_egp  rle_egp.cpp debug_egprle.o

debug_unrle_egp: debug_egprle.o unrle_egp.cpp
	g++ ${DEBUG_FLAGS} -o debug_unrle_egp unrle_egp.cpp debug_egprle.o

bench_rle_egp: bench_egprle.o rle_egp.cpp
	g++ ${BENCH_FLAGS} -o bench_rle_egp  rle_egp.cpp bench_egprle.o

bench_unrle_egp: bench_egprle.o unrle_egp.cpp
	g++ ${BENCH_FLAGS} -o bench_unrle_egp unrle_egp.cpp bench_egprle.o

egprle.o: egprle.h egprle.cpp circular_buffer.h utils.h types.h
	g++ ${CPPFLAGS} -c egprle.cpp

debug_egprle.o: egprle.h egprle.cpp circular_buffer.h utils.h types.h
	g++ ${DEBUG_FLAGS} -c -o debug_egprle.o egprle.cpp

bench_egprle.o: egprle.h egprle.cpp circular_buffer.h utils.h types.h
	g++ ${BENCH_FLAGS} -c -o bench_egprle.o egprle.cpp

clean:
	rm -f egprle.o unrle_egp rle_egp debug_rle_egp debug_unrle_egp debug_egprle.o bench_rle_egp bench_unrle_egp bench_epgrle.o

test: ./test/test.cpp
	g++ ${CPPFLAGS} -o test_exec ./test/test.cpp
	./test_exec
	@rm -f test_exec

