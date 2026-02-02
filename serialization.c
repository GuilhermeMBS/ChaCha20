#include "serialization.h"


void serialize_state(uint32_t* state, uint8_t* key_stream) {
    uint32_t mask[16] = { 0 };
    for (int i = 0; i < 16; i++) mask[i] = state[i];

    for (int i = 0; i < 64; i++) {
        int index = i/4;
        key_stream[i] |= mask[index] & 0xFF;
        mask[index] >>= 8;
    }

    return;
}


void chacha20_encrypt(uint8_t* key, uint8_t* block, uint8_t* nonce, FILE* plaintext, FILE* encrypted_message) {
    uint8_t data_stream[64] = { 0 };
    uint8_t key_stream[64] = { 0 };

    int counter = 0;
    int character;
    
    while ((character = fgetc(plaintext)) != EOF) {
        
        data_stream[counter++] = character;

        if (counter == 64) {
            uint32_t state[16] = {
                0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
                0x00000000, 0x00000000, 0x00000000, 0x00000000,
                0x00000000, 0x00000000, 0x00000000, 0x00000000,
                0x00000000, 0x00000000, 0x00000000, 0x00000000,
            };

            chacha20_block(state, key, block, nonce);
            serialize_state(state, key_stream);
           
            for (int i = 0; i < 64; i++) {
                key_stream[i] ^= data_stream[i];
                fputc(key_stream[i], encrypted_message);
                key_stream[i] = 0;
                data_stream[i] = 0;
            }

            counter = 0;
            add_block(block);
        }

    }

    if (counter != 0) {
        uint32_t state[16] = {
            0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
        };

        chacha20_block(state, key, block, nonce);
        serialize_state(state, key_stream);
        
        for (int i = 0; i < counter; i++) {
            key_stream[i] ^= data_stream[i];
            fputc(key_stream[i], encrypted_message);
            key_stream[i] = 0;
            data_stream[i] = 0;
        }

        counter = 0;
        (*(uint32_t*)block)++;
    }


    return;
}