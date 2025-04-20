#include "util.hpp"
#include <fcntl.h>
#include <sys/mman.h>

// Flush+Reload utilities
CYCLES measure_one_block_access_time(ADDR_PTR addr) {
    CYCLES cycles;
    asm volatile("mov %1, %%r8\n\t"
                 "lfence\n\t"
                 "rdtsc\n\t"
                 "mov %%eax, %%edi\n\t"
                 "mov (%%r8), %%r8\n\t"
                 "lfence\n\t"
                 "rdtsc\n\t"
                 "sub %%edi, %%eax\n\t"
                 : "=a"(cycles)
                 : "r"(addr)
                 : "r8", "edi");
    return cycles;
}

uint64_t RDTSC() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

void CLFLUSH(ADDR_PTR addr) {
    asm volatile ("clflush (%0)"::"r"(addr));
}

uint64_t cache_set_index(ADDR_PTR addr) {
    uint64_t mask = ((uint64_t)1 << 16) - 1;
    return (addr & mask) >> 6;
}

char* convert_to_binary(char* input) {
    size_t length = strlen(input);
    char* output = (char*)malloc(length * 8 + 1);
    output[0] = '\0';

    for (int i = 0; i < length; ++i) {
        for (int j = 7; j >= 0; --j) {
            strcat(output, (input[i] & (1 << j)) ? "1" : "0");
        }
    }
    return output;
}

char* convert_from_binary(char* bitstring, int bitstring_size) {
    int num_chars = bitstring_size / 8;
    char* message = (char*)malloc(num_chars + 1);
    int idx = 0;

    for (int i = 0; i < num_chars; i++) {
        char eight_bits[9] = {0};
        strncpy(eight_bits, &bitstring[idx], 8);
        message[i] = strtol(eight_bits, NULL, 2);
        idx += 8;
    }
    message[num_chars] = '\0';
    return message;
}

// Lab 8 helpers
void* get_shared_mem() {
    const char* path = "/dev/shm/covert_shared";
    int fd = open(path, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    ftruncate(fd, CACHE_LINESIZE);
    void* addr = mmap(NULL, CACHE_LINESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);
    return addr;
}

void access(void* addr) {
    volatile uint8_t* p = (volatile uint8_t*)addr;
    *p;
}

void flush(void* addr) {
    asm volatile("clflush 0(%0)\n" : : "r"(addr) : "memory");
}

uint64_t rdtscp() {
    uint32_t lo, hi;
    asm volatile ("rdtscp" : "=a"(lo), "=d"(hi) : : "rcx");
    return ((uint64_t)hi << 32) | lo;
}
