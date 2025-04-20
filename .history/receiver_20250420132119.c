#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.hpp"

#define BIT_DELAY 500000 // microseconds
#define THRESHOLD 300    // nanoseconds

char recv_bit(void *addr) {
    uint64_t t1 = rdtscp();
    cache_access(addr);
    uint64_t t2 = rdtscp();
    flush(addr);
    usleep(BIT_DELAY);
    return (t2 - t1 < THRESHOLD) ? '1' : '0';
}

void recv_message(int bits, void *addr) {
    char *bitstring = (char *)malloc(bits + 1);
    bitstring[bits] = '\0';

    for (int i = 0; i < bits; i++) {
        bitstring[i] = recv_bit(addr);
        fflush(stdout);
    }

    char *decoded = convert_from_binary(bitstring, bits);
    printf("Received: %s\n", decoded);

    free(bitstring);
    free(decoded);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <num_bits>\n", argv[0]);
        return 1;
    }

    int bits = atoi(argv[1]);
    void *addr = get_shared_mem();

    printf("Receiving %d bits...\n", bits);
    printf("Press ENTER to start listening...\n");
    getchar();  // Wait for sender to be ready

    recv_message(bits, addr);

    return 0;
}
