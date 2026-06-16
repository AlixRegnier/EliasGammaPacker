CPPFLAGS=-O3 -std=c++17 -march=native -mavx2 -Wall
DEBUG_FLAGS=-O0 -g -std=c++17 -march=native -mavx2 -Wall

all: rle_egp unrle_egp

debug: debug_rle_egp debug_unrle_egp

rle_egp: egprle.o rle_egp.cpp
	g++ ${CPPFLAGS} -o rle_egp  rle_egp.cpp egprle.o

unrle_egp: egprle.o unrle_egp.cpp
	g++ ${CPPFLAGS} -o unrle_egp unrle_egp.cpp egprle.o

debug_rle_egp: debug_egprle.o rle_egp.cpp
	g++ ${DEBUG_FLAGS} -o debug_rle_egp  rle_egp.cpp debug_egprle.o

debug_unrle_egp: debug_egprle.o unrle_egp.cpp
	g++ ${DEBUG_FLAGS} -o debug_unrle_egp unrle_egp.cpp debug_egprle.o

test: ./test/test.cpp
	g++ ${CPPFLAGS} -o test_exec ./test/test.cpp
	./test_exec
	@rm -f test_exec

egprle.o: egprle.h egprle.cpp circular_buffer.h utils.h types.h
	g++ ${CPPFLAGS} -c egprle.cpp

debug_egprle.o: egprle.h egprle.cpp circular_buffer.h utils.h types.h
	g++ ${DEBUG_FLAGS} -c -o debug_egprle.o egprle.cpp

clean:
	rm -f epgrle.o unrle_egp rle_egp debug_rle_egp debug_unrle_egp debug_egprle.o

try: debug_rle_egp debug_unrle_egp
	./debug_rle_egp ./data/matrix_148_ecoli_reordered.cmbf out.rle
	./debug_unrle_egp out.rle out.cmbf
