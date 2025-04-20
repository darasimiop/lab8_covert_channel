#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.hpp"

#define BIT_DELAY 500000 // microseconds

void send_bit(int bit, void *addr) {
    if (bit == 1) {
        cache_access(addr);
    }
    usleep(BIT_DELAY);
    flush(addr);
}

void send_message(const char *msg, void *addr) {
    for (int i = 0; i < strlen(msg); i++) {
        char ch = msg[i];
        for (int bit = 7; bit >= 0; bit--) {
            send_bit((ch >> bit) & 1, addr);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <1-char message>\n", argv[0]);
        return 1;
    }

    const char *message = argv[1];
    if (strlen(message) != 1) {
        printf("Error: please send exactly one character.\n");
        return 1;
    }

    void *addr = get_shared_mem();

    printf("Sending: %s\n", message);
    printf("Press ENTER to begin transmission...\n");
    getchar();  // Give receiver time to be ready

    send_message(message, addr);

    return 0;
}
