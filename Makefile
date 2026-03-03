all:
	g++ -O3 -march=native -mavx2 -std=c++17 rle_egc.cpp -o rle_egc
	g++ -O3 -march=native -mavx2 -std=c++17 unrle_egc.cpp -o unrle_egc
