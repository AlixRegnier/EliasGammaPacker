CPPFLAGS=-O3 -std=c++14 -march=native -mavx2 -Wall #-DEGP_NOCHECK

all: rle_egp unrle_egp

rle_egp:   bitpacker.o rle_egp.cpp elias_gamma_packer.h
	g++ ${CPPFLAGS} -o rle_egp   rle_egp.cpp bitpacker.o

unrle_egp: bitpacker.o unrle_egp.cpp elias_gamma_packer.h
	g++ ${CPPFLAGS} -o unrle_egp unrle_egp.cpp bitpacker.o

test: bitpacker.o elias_gamma_packer.h
	g++ ${CPPFLAGS} -o test_exec ./test/test.cpp bitpacker.o
	@./test_exec
	@rm -f test_exec

bitpacker.o: bitpacker.cpp bitpacker.h
	g++ ${CPPFLAGS} -c bitpacker.cpp

simd: egprle.h egprle.cpp circular_buffer.h
	g++ ${CPPFLAGS} -o simd_egp egprle.cpp
clean:
	rm -f bitpacker.o unrle_egp rle_egp