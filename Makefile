CPPFLAGS=-O3 -std=c++17 -march=native -mavx2 -Wall #-DEGP_NOCHECK

all: rle_egp unrle_egp

rle_egp:   egprle.o rle_egp.cpp elias_gamma_packer.h
	g++ ${CPPFLAGS} -o rle_egp   rle_egp.cpp egprle.o

unrle_egp: egprle.o unrle_egp.cpp elias_gamma_packer.h
	g++ ${CPPFLAGS} -o unrle_egp unrle_egp.cpp egprle.o

test: ./test/test.cpp
	g++ ${CPPFLAGS} -o test_exec ./test/test.cpp
	./test_exec
	@rm -f test_exec

egprle.o: egprle.h egprle.cpp circular_buffer.h
	g++ ${CPPFLAGS} -c egprle.cpp

tmp: egprle.h egprle.cpp circular_buffer.h
	g++ ${CPPFLAGS} -o tmp egprle.cpp

clean:
	rm -f epgrle.o unrle_egp rle_egp