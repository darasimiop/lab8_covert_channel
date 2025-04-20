#include "util.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

extern "C" {

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

void* get_shared_mem() {
    const char* path = "/dev/shm/covert_shared";
    int fd = open(path, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    if (ftruncate(fd, CACHE_LINESIZE) != 0) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    void* addr = mmap(NULL, CACHE_LINESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
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

}
