#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.hpp"

#define BIT_DELAY 500000 // microseconds

void send_bit(int bit, void *addr) {
    flush(addr);  // Clear cache line first
    if (bit == 1) {
        cache_access(addr);  // Bring into cache
    }
    usleep(BIT_DELAY);
}

void send_sync(void *addr) {
    printf("[DEBUG] Sending sync byte 0xAA...\n");
    for (int i = 7; i >= 0; i--) {
        int bit = (0xAA >> i) & 1;
        printf("[DEBUG] Sync bit: %d\n", bit);
        send_bit(bit, addr);
    }
}

void send_message(const char *msg, void *addr) {
    for (int i = 0; i < strlen(msg); i++) {
        char ch = msg[i];
        printf("[DEBUG] Character: '%c' -> ", ch);
        for (int bit = 7; bit >= 0; bit--) {
            int b = (ch >> bit) & 1;
            printf("%d", b);
            printf("\n[DEBUG] Sending bit: %d\n", b);
            send_bit(b, addr);
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
    getchar();  // Wait for receiver to be ready

    send_sync(addr);
    send_message(message, addr);

    return 0;
}
