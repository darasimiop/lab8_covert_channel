#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.hpp"

#define BIT_DELAY 100000 // microseconds
#define THRESHOLD 150    // nanoseconds

char recv_bit(void *addr) {
    uint64_t t1 = rdtscp();
    cache_access(addr);
    uint64_t t2 = rdtscp();
    flush(addr);
    usleep(BIT_DELAY);
    return (t2 - t1 < THRESHOLD) ? '1' : '0';
}

void recv_message(int bits, void *addr) {
    for (int i = 0; i < bits; i++) {
        printf("%c", recv_bit(addr));
        fflush(stdout);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <num_bits>\n", argv[0]);
        return 1;
    }

    int bits = atoi(argv[1]);
    void *addr = get_shared_mem();

    printf("Receiving %d bits...\n", bits);
    printf("Waiting 3 seconds before starting to listen...\n");
    sleep(3);  // Delay to give sender time to start

    recv_message(bits, addr);

    return 0;
}
