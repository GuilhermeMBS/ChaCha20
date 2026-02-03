#ifndef CHACHA_H
#define CHACHA_H

#include <stdio.h>
#include <stdint.h>

#define MATRIX_SIZE 16
#define KEY_SIZE 32
#define COUNTER_SIZE 4
#define NONCE_SIZE 12

void chacha20_encrypt(uint8_t* key, uint8_t* counter, uint8_t* nonce, FILE* plaintext, FILE* encrypted_message);

#endif