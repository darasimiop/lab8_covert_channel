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

#define PERIOD 500
#define DECISION_BOUNDARY 150
#define TWO 2
#define MULTIPLIER 2
#define MESSAGE_MAX 128
#define BYTE_BIT 8
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
}

#endif
