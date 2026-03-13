#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <immintrin.h>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cstring>

#include "elias_gamma_packer.h"


int main(int argc, char ** args) {

    if(argc != 3)
    {
        std::cout << "Usage: unrle_egp <input> <output>\n" << std::endl;
        return 1;
    }

    std::string input_filename = args[1];
    std::string output_filename = args[2];

    EliasGammaPacker egp;
    egp.deserialize(input_filename);

	std::cout << egp.get_byte_count() << std::endl;
	const std::uint64_t file_size = egp.get_file_size();

    int fd = open(output_filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
	{
        std::cerr << "main : couldn't open file '" << input_filename << '\'' << std::endl;
		return 2;
	}

	std::cout << file_size << std::endl;

    char* map = (char*)mmap(nullptr, file_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        std::cerr << "main : mmap initialization failed" << std::endl;
		return 2;
    }

    posix_madvise(map, file_size, MADV_SEQUENTIAL);

	const std::uint64_t nb_packed_values = egp.get_packed_values_count();
	std::uint64_t bit_pos = 0;
	std::uint64_t run_length = 0;
	
	std::uint64_t i = 0;
	
	if(!egp.get_first_bit() && nb_packed_values > 0)
	{
		bit_pos = egp.unpack(); //Unpack first run of 0s
		i = 1;
	}

	const std::uint8_t FF = 0xFF;
	
	for(; i+1 < nb_packed_values; i += 2)
	{
		run_length = egp.unpack(); //Get first bit run of 1s
		
		// Handle first unaligned partial byte if any
		if(bit_pos % 8 != 0)
		{
			//Update first unaligned partial byte
			map[bit_pos/8] = FF >> (bit_pos % 8);

			//Update run-length
			run_length -= 8 - (bit_pos % 8);

			//Update bit position to next byte
			bit_pos = bit_pos/8*8 + 1;
		}

		// Handle aligned bytes 
		for(std::uint64_t j = 0; j < run_length / 8; ++j, bit_pos += 8)
			map[bit_pos/8] = FF;

		// Handle last unaligned partial byte if any
		if(run_length % 8 != 0)
			map[bit_pos/8] = FF << (run_length % 8);

		bit_pos += run_length + egp.unpack(); //Update current bit position by the previously number of added 1s and add also the of next 0s
	}

	//Handle last run of 1s 
	if(egp.get_first_bit() == nb_packed_values % 2)
	{
		run_length = egp.unpack(); //Get first bit run of 1s
			
		// Handle first unaligned partial byte if any
		if(bit_pos % 8 != 0)
		{
			//Update first unaligned partial byte
			map[bit_pos/8] = FF >> (bit_pos % 8);

			//Update run-length
			run_length -= 8 - (bit_pos % 8);

			//Update bit position to next byte
			bit_pos = bit_pos/8*8 + 1;
		}

		// Handle aligned bytes 
		for(std::uint64_t j = 0; j < run_length / 8; ++j, bit_pos += 8)
			map[bit_pos/8] = FF;

		// Handle last unaligned partial byte if any
		if(run_length % 8 != 0)
			map[bit_pos/8] = FF << (run_length % 8);

		//bit_pos += run_length + egp.unpack(); //Update current bit position by the previously number of added 1s and add also the of next 0s
	}
	
	munmap(map, file_size);
    close(fd);

    return 0;
}
