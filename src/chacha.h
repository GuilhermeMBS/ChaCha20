#ifndef CHACHA_H
#define CHACHA_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define KEY_SIZE 32
#define COUNTER_SIZE 4
#define NONCE_SIZE 12

typedef struct {
    uint32_t state[16];
    uint8_t keystream[64];
    size_t keystream_idx;
} chacha20_ctx;


void chacha20_encrypt(uint8_t* key, uint8_t* counter, uint8_t* nonce, FILE* plaintext, FILE* encrypted_message);

#endif