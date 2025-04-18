
// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print 
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions 
// (e.g., rand()).  You may not use system().
#include <stdlib.h>

#include <inttypes.h>
#include <time.h>
#include <cstring> 
#include <iostream>
#include <unistd.h>
#include <list>
#include <math.h>

using namespace std; 


#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

#define PERIOD 500
#define DECISION_BOUNDARY 150
#define TWO 2
#define MULTIPLIER 2
#define MESSAGE_MAX 128
#define BYTE_BIT 8

// CONSTANTS for system
// get these from % getconf -a | grep CACHE
const int CACHE_LINESIZE = 64;

//const int CACHE_L1_SIZE = 32768;  //my desktop
//const int CACHE_L1_ASSOC = 8; // my desktop
const int CACHE_L1_SIZE = 49152;  //my laptop
const int CACHE_L1_ASSOC = 12;
const int CACHE_L1_SETS = (CACHE_L1_SIZE / CACHE_LINESIZE) / CACHE_L1_ASSOC;

//const int CACHE_L3_SIZE = 8388608; // 8MB (my desktop)
//const int CACHE_L3_ASSOC = 16; // my desktop
const int CACHE_L3_SIZE = 12582912;  // 12MB
const int CACHE_L3_ASSOC = 12; // i7-1265U (my laptop)
const int CACHE_L3_SETS = (CACHE_L3_SIZE / CACHE_LINESIZE) / CACHE_L3_ASSOC;
// my desktop
// const int CACHE_L3_SLICES = 4;  // 4 cores in a socket each core has a L3 slice
const int CACHE_L3_SLICES = 10;  // there are 2P+8E cores in a socket each core has a L3 slice (my laptop)

CYCLES measure_one_block_access_time(ADDR_PTR addr);

void CLFLUSH(ADDR_PTR addr);

uint64_t RDTSC();

uint64_t cache_set_index(ADDR_PTR addr);

char* convert_to_binary(char* input);

char* convert_from_binary(char* bitstring, int bitstring_size);

#endif
