#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <list>
#include <math.h>

using namespace std;

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t
#define CACHE_LINESIZE 64

extern "C" {
    CYCLES measure_one_block_access_time(ADDR_PTR addr);
    void CLFLUSH(ADDR_PTR addr);
    uint64_t RDTSC();
    uint64_t cache_set_index(ADDR_PTR addr);
    void* get_shared_mem();
    void cache_access(void* addr);
    void flush(void* addr);
    uint64_t rdtscp();
    char* convert_from_binary(char* bitstring, int bitstring_size);
}

#endif
