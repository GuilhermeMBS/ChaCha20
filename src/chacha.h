#ifndef CHACHA_H
#define CHACHA_H

#include <stdint.h>

#define KEY_SIZE 32
#define COUNTER_SIZE 4
#define NONCE_SIZE 12

typedef struct {
    uint32_t state[16];
    uint8_t keystream[64];
    size_t keystream_idx;
} chacha20_ctx;


void chacha20_init(chacha20_ctx* ctx, uint8_t* key, uint8_t* counter, uint8_t* nonce);
void chacha20_update(chacha20_ctx* ctx, uint8_t* input, uint8_t* output, size_t length);
void chacha20_wipe(chacha20_ctx* ctx);

#endif